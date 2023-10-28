from cs50 import get_int

# Asks the user what the height of the semi-pyramid is, (between 1 and 8)
while True:
    height = get_int("Height: ")
    if 1 <= height <= 8:
        break

# Shows the pyramid blocks on the screen
for c in range(height):
    c += 1
    print(" " * (height - c) + "#" * c)
