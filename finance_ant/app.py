import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Retrieve user id
    user_id = session["user_id"]

    # Query user stocks and shares
    stocks = db.execute(
        "SELECT symbol, sum(shares) as total_shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING total_shares>0",
        user_id=session["user_id"])

    # Get user's cash balance
    cash = db.execute("SELECT cash from users WHERE id =:user_id", user_id=session["user_id"])[0]["cash"]

    #Inicialize variables for total values
    total_value = cash
    grand_value = cash

    # Iterate over stocks and add pric and total value
    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["name"] = quote["name"]
        stock["price"] = quote["price"]
        stock["value"] = stock["price"] * stock["total_shares"]
        total_value += stock["value"]
        grand_value += stock["value"]

    return render_template("index.html", stocks=stocks, cash=cash, total_value=total_value, grand_value=grand_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # Retrieve user id


    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        if not symbol:
            return apology("must provide symbol")
        elif not shares or not shares.isdigit() or int(shared) <=0:
            return apology("must provide a positive integer number of shares")

        quote = lookup(symbol)

        if qoute is None:
            return apology("symbol not found")

        price = quote["price"]
        total_cost = int(shares) * price
        cash = db.execute("Select cash from users where id= : user_id", user_id=session["user_id"])[0]["Cash"]

        if cash < total_cost:
            return apology("symbol not found")

        # Update users table
        db.execute("Update users Set cash = cash - : total_cost Where id = : user_id", total_cost = total_cost, user_id=session["user_id"])

        # Add the purchase to the history table
        db.execute("Insert into transaccions(user_id, symbol, shares, price) Values(:user_id, :symbol, :shares, :price)",
                   user_id=session["user_id"], symbol=symbol, shares=shares, price=price)

        flash(f"Bought {shares} shares of {symbol} for {usd(total_cost)}!")
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Retrieve user id
    user_id = session["user_id"]

    # Query database for stransactions hisotry
    transactions = db.execute(
        "SELECT * FROM  transactions WHERE user_id = ? ORDER BY date DESC;",
        user_id,
    )
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        # Ensure symbol was submitted
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if not quote:
            return apology("Invalid symbol",400)
        return render_template("quote.html",quote=quote)
    else:
        return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation password was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide confirmation password", 400)

        # Retrive submitted data
        else:
            username = request.form.get("username")
            password = request.form.get("password")
            confirmation = request.form.get("confirmation")

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username is not already in use and the two password matches
        if len(rows) != 0:
            return apology("invalid username", 400)
        elif password != confirmation:
            return apology("invalid confirmation password", 400)

        # Generate password hash
        hash = generate_password_hash(request.form.get("password"))

        # Add the new user into the database
        db.execute("INSERT INTO users (username,hash) VALUES (?,?)", username, hash)

        # Redirect user to login form
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Retirve user id
    user_id = session["user_id"]

    # Query database for portfolio
    shares = db.execute(
        "SELECT symbol FROM  transactions WHERE user_id = ?",
        user_id,
    )

    if request.method == "POST":
        # Ensure share was submitted
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)
        else:
            symbol = request.form.get("symbol")
            # Check if the symbol is valid
            quote = get_quote_info(symbol)
            if quote is None:
                return apology("invalid symbol", 400)
            else:
                symbol = quote["symbol"]

        # Ensure number of share was submitted
        if not request.form.get("shares"):
            return apology("missing shares", 400)
        else:
            # Check if the number of shares is valid
            shares = try_parse_int(request.form.get("shares"))
            if shares is None or shares <= 0:
                return apology("inavlid number of shares", 400)

        # Query database for number of shares
        row = db.execute(
            "SELECT shares FROM transactions WHERE user_id = ? AND symbol = ?",
            user_id,
            symbol,
        )
        total_shares = int(row[0]["shares"])

        # Check the feasibility of the operation
        if shares > total_shares:
            return apology("too many shares", 400)
        else:
            total_shares -= shares

        # Update the Portfolio
        if total_shares == 0:
            db.execute(
                "DELETE FROM transactions WHERE user_id = ? AND symbol = ?",
                user_id,
                symbol,
            )
        else:
            db.execute(
                "UPDATE transactions SET shares = ? WHERE user_id = ? AND symbol = ?",
                total_shares,
                user_id,
                symbol,
            )

        # Update transactions book
        db.execute(
            "INSERT INTO transactions (user_id,symbol,shares,price) VALUES (?,?,?,?)",
            user_id,
            symbol,
            -shares,
            quote["price"],
        )

        # Update cash on the account
        cash = get_cash(user_id)
        received = shares * quote["price"]
        cash += received
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?",
            cash,
            user_id,
        )

        # Redirect user to home page
        return redirect("/")
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html", shares=shares)
