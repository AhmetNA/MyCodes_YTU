class Account:
    def __init__ (self , account_number , balance , account_holder_name):
        self.account_number = account_number
        self.balance = balance
        self.account_holder_name = account_holder_name

    def deposit(self , amount):
        if amount <= 0:
            print("Invalid deposit amount. Please try again.")
            return False
        self.balance += amount
        print(f"Amount deposited: ${amount:.2f} to account {self.account_number}.")
        return self.balance
            
    def withdraw(self , amount):
        if amount <= 0:
            print("Invalid withdraw amount. Please try again.")
            return False
        if amount > self.balance:
            print("Insufficient funds. Please try again.")
            return False
        self.balance -= amount
        print(f"Amount withdrawn: ${amount:.2f} from account {self.account_number}.")
        return self.balance
    
    def __str__(self):
        return f"Account number: {self.account_number}\nAccount holder: {self.account_holder_name}\nAccount balance: ${self.balance:.2f}"

class SavingsAccount(Account):
    def __init__(self , account_number , balance , account_holder_name , interest_rate):
        super().__init__(account_number , balance , account_holder_name)
        self.interest_rate = interest_rate
    
    def calculate_interest(self):
        interest = self.balance * self.interest_rate / 100
        self.balance += interest
        print(f"Interest added: ${interest} to account {self.account_number}.")
        return interest
    
    def __str__(self):
        return f"Interest rate: {self.interest_rate}"

def main():
    account1 = Account(123 , 1000 , "John Doe")
    print(account1)
    account1.deposit(500)
    account1.withdraw(200)
    print(account1)
    
    account2 = SavingsAccount(456 , 2000 , "Jane Doe" , 2)
    print(account2)
    account2.deposit(1000)
    account2.withdraw(500)
    account2.calculate_interest()
    print(account2)

if __name__ == "__main__":
    main()

