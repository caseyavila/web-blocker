import os.path
import os
import random

passWord = str(random.randint(0, 100))
guess = ''
var = ''


def ban():
    os.rename('hosts_ban', 'temp')
    os.rename('hosts', 'hosts_clean')
    os.rename('temp', 'hosts')


def unban():
    os.rename('hosts_clean', 'temp')
    os.rename('hosts', 'hosts_ban')
    os.rename('temp', 'hosts')

def password():
    global guess, var
    while guess != passWord:
        guess = (input("Password: "))
        try:
            int(guess)

        except ValueError:
            print("Please enter an integer.")

# Set directory to host files
os.chdir('C:\Windows\System32\drivers\etc')

while var != "b" or "u":
    # Receive input on whether to ban on unban
    var = input("Ban or unban? (b/u): ")

    # user chooses to ban
    if var == "b":

        # ban user unless already banned
        if os.path.exists('hosts_ban'):
            try:
                ban()
            except PermissionError:
                wait = input("Please launch with administrator privileges, press ENTER to terminate.")
                raise SystemExit()
            input("You have been banned, press ENTER to terminate.")
            raise SystemExit()
        else:
            print("You are already banned.")

    # user chooses to unban
    elif var == "u":
        password() # prompt password

        # unban user unless already unbanned
        if os.path.exists('hosts_clean'):
            try:
                unban()
            except PermissionError:
                input("Please launch with administrator privileges, press ENTER to terminate.")
                raise SystemExit()
            input("You have been unbanned, press ENTER to terminate.")
            raise SystemExit()
        else:
            print("You are already unbanned.")

    # notify user to use "b" and "u" to choose an option
    else:
        print("Invalid choice. Type \"b\" to ban, and \"u\" to unban.")
