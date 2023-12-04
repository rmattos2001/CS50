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
    stocks = db.execute("SELECT symbol, shares FROM stocks WHERE user_id = ?;", session["user_id"])
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"])[0]["cash"]

    stocks_total_price = 0
    for stock in stocks:
        stocks_total_price += lookup(stock["symbol"])["price"] * stock["shares"]

    return render_template("index.html", user_cash=user_cash, lookup=lookup, stocks=stocks, stocks_total_price=stocks_total_price)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Search for the stock
        stock = lookup(symbol)

        # Ensure there is a stock with that symbol
        if not stock:
            return apology("invalid symbol", 400)

        # Ensure shares was submitted
        elif not shares:
            return apology("missing shares", 400)

        # Ensure shares is a positive number
        elif not shares.isdigit() or int(shares) < 1:
            return apology("invalid shares", 400)

        user_cash = db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"])[0]["cash"]
        shares_total_price = stock["price"] * int(shares)

        # Ensure user has enough money to buy shares
        if not user_cash > shares_total_price:
            return apology("can't afford")

        # Insert the new purchase into transactions, along with the stock symbol and price
        db.execute("INSERT INTO transactions (user_id, symbol, price, shares, type) VALUES (?, ?, ?, ?, 'purchase');",
                   session["user_id"], stock["symbol"], stock["price"], shares)

        # Insert the stocks bought into stocks, along with the stock symbol
        db.execute("INSERT INTO stocks (user_id, symbol, shares) VALUES (?, ?, ?);", session["user_id"], stock["symbol"], shares)

        # Subtract user cash used into that purchase
        db.execute("UPDATE users SET cash = ? WHERE id = ?;", user_cash - shares_total_price, session["user_id"])

        # Success message
        flash('Bought!')

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT symbol, shares, price, type, date FROM transactions WHERE user_id = ?;", session["user_id"])

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
    return apology("TODO")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    return apology("TODO")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    return apology("TODO")
