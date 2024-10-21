import java.util.*;

public class Main
{
   static Map<String, Trader> registeredUser = new HashMap<String, Trader>();
   static Map<String, Trader> users = new HashMap<String, Trader>();
   
   static int MainMenu()
   {
       System.out.println(users);

       Scanner input = new Scanner(System.in);

    System.out.println("-------------------------");
    System.out.println("Welcome to the main menu.");
    System.out.println("Select an option:");
    System.out.println("-------------------------");
    System.out.println("1 - Register an account");
    System.out.println("2 - Log in to your account");
    System.out.println("3 - Exit the system");
    System.out.println("-------------------------");
    System.out.println("Your Choice: ");

    int numChoice = 0;

    try
    {
        String userChoice = input.nextLine();
        numChoice = Integer.parseInt(userChoice.trim());
    }
    catch(NumberFormatException nfe)
    {
        System.out.println(nfe.getMessage());
    }

    return numChoice;
   }

   static int TraderMenu()
   {
        Scanner input = new Scanner(System.in);

        System.out.println("Welcome to the Trader's menu.");
        System.out.println("Please select an option.");
        System.out.println("-------------------------"); 
        System.out.println("1 - View OrderBooks");
        System.out.println("2 - Buy Crypto");
        System.out.println("3 - Sell Crypto");
        System.out.println("4 - View Crypto Transaction");
        System.out.println("5 - Deposit Money");
        System.out.println("6 - View Balance and Wallet");
        System.out.println("7 - Log out of your account.");
        System.out.println("-------------------------");
        System.out.print("Your Choice: ");

        int numChoice = 0;

        try
        {
            String traderChoice = input.nextLine();
            numChoice = Integer.parseInt(traderChoice.trim());
        }
        catch(NumberFormatException nfe)
        {
            System.out.println(nfe.getMessage());
        }

        return numChoice;
    }

    static int AdminMenu()
    {
        Scanner input = new Scanner(System.in);
        System.out.println("-------------------------");
        System.out.println("This is the Admin's Menu.");
        System.out.println("Please select an option:");
        System.out.println("-------------------------");
        System.out.println("1 - Approve Users");
        System.out.println("2 - Log out of your account");
        System.out.println("-------------------------");
        System.out.println("Your choice: ");

        int numChoice = 0;

        try
        {
            String adminChoice = input.nextLine();
            numChoice = Integer.parseInt(adminChoice.trim());
        }
        catch(NumberFormatException nfe)
        {
            System.out.println(nfe.getMessage());
        }

        return numChoice;
    }

    //Main Method
    public static void main(String[] args)
    {
        Scanner input = new Scanner(System.in);
        int choice;

        boolean exitted = false;
        boolean loggedIn = false;
        boolean isTrader = false;
        boolean userFound = false;

        UUID currentUser = null;

        //Test cases
        UUID adminUUID = UUID.randomUUID();
        User adminUser = new User("Admin", "Admin", adminUUID);

        UUID testUUID1 = UUID.randomUUID();
        Trader testUser1 = new Trader("Benjamin", "1999", testUUID1, 300.0, new ArrayList<String>(), new HashMap<String, Integer>());
        users.put("Benjamin", testUser1);

        UUID testUUID2 = UUID.randomUUID();
        Trader testUser2 = new Trader("Beppe", "1999", testUUID2, 420.0, new ArrayList<String>(), new HashMap<String, Integer>());
        users.put("Beppe", testUser2);

        testUser1.initOrderBook();

        //End test cases

        while(!exitted)
        {
            if(!loggedIn)
            {
                choice = MainMenu();

                switch(choice){
                    case 1:
                        UUID uniqueKey = UUID.randomUUID();
                        //Register an Account
                        boolean taken = true;

                        String nUsername;

                        do
                        {
                            System.out.print("Enter Username: ");
                            nUsername = input.nextLine();

                            if(registeredUser.containsKey(nUsername) || users.containsKey(nUsername) || nUsername.equals("admin") || nUsername.equals("Admin"))
                            {
                                taken = true;
                                System.out.println("Username is already in use, try again"); //If username is already in use, a different username must be used
                            }
                            else
                            {
                                taken = false;
                            }
                        }while(taken);

                        System.out.println("Enter Password: ");
                        String nPassword = input.nextLine();

                        try
                        {
                            Trader trader = new Trader(nUsername, nPassword, uniqueKey, 0.0, new ArrayList<String>(), new HashMap<String, Integer>());
                            registeredUser.put(nUsername, trader);
                        }catch (Exception e)
                        {
                            System.out.println(e);
                        }

                        System.out.println("Trader Account has been Created. Please wait for admin's approval.");
                    break;

                    case 2:
                        //Log in to account
                        String LogInUsername;
                        String LogInPassword;

                        System.out.print("Enter Username: ");
                        LogInUsername = input.nextLine();
                        System.out.print("Enter Password: ");
                        LogInPassword = input.nextLine();

                        if(LogInUsername.equals("Admin") && LogInPassword.equals("Admin")) //to check if Admin is logging in
                        {
                            loggedIn = true;
                            isTrader = false;
                        }
                        else
                        {
                            currentUser = null;

                            for(Map.Entry<String, Trader> trader: users.entrySet())
                            {
                                String entryname = trader.getKey();
                                if(entryname.equals(LogInUsername))
                                {
                                    userFound = true;
                                    User entryuser = trader.getValue();
                                    String entrypass = entryuser.getPassword();

                                    if(entrypass.equals(LogInPassword))
                                    {
                                        System.out.println("Login Successful.");
                                        currentUser = entryuser.getUserId();
                                        loggedIn = true;;
                                        isTrader = true;
                                        break;
                                    }
                                    else
                                    {
                                        System.out.println("Login failed.");
                                    }
                                }
                            }

                            if(currentUser == null)
                            {
                                System.out.println("No user found.  Please register an account."); //If account doesn't exist access is denied.
                            }
                        }
                    break;

                    case 3:
                        //To exit
                        exitted = true;
                    break;

                    default:
                    break;
                }
            }
            else
            {
                if(isTrader)
                {
                    Trader loggedUser = null;

                    for(Map.Entry<String, Trader> trader : users.entrySet())
                    {
                        Trader tradertocheck = trader.getValue();
                        if(currentUser == tradertocheck.getUserId())
                        {
                            loggedUser = tradertocheck;
                            break;
                        }
                    }

                    if(loggedUser != null)
                    {
                        System.out.println("User ID: " + currentUser + " " + loggedUser.getUsername());

                        do
                        {
                            choice = TraderMenu();
                            
                            switch(choice)
                            {
                                case 1:
                                    //View Order Book
                                    System.out.println("~~~~~~~~~ Order Book ~~~~~~~~~");
                                    loggedUser.displayOrderBook();

                                    System.out.println("~~~~~~~~~ Users Wallets ~~~~~~~~~");
                                    System.out.println(loggedUser.getWallet());
                                break;

                                case 2:

                                    //To buy Crypto
                                    boolean marlimValid = false;
                                    String marlimChoice;

                                    do
                                    {
                                        System.out.print("\nEnter 'Market' Order or 'Limit' Order: ");
                                        marlimChoice = input.nextLine();

                                        if(marlimChoice.equals("Market"))
                                        {
                                            marlimValid = true;
                                        }
                                        else if(marlimChoice.equals("Limit"))
                                        {
                                            marlimValid = true;
                                        }
                                        else
                                        {
                                            System.out.println("Wrong Input.  Please try again. \n");
                                        }
                                    }while(!marlimValid);

                                    switch(marlimChoice)
                                    {
                                        case "Market": //Will pass crypto name + quantity, loop through the asks, and get the cheapest choice
                                            boolean marketChoice = false;

                                            do
                                            {
                                                System.out.print("-----------------------------------------\n");
                                                loggedUser.viewCryNames();
                                                System.out.print("Enter what crypto you'd like to buy from the Market: ");
                                                String cryptoChoice = input.nextLine();
                                                System.out.print("Enter the amount you'd like to buy: ");
                                                int cryptoQTY = input.nextInt();

                                                if(cryptoChoice.equals("Bitcoin") || cryptoChoice.equals("Ethereum") || cryptoChoice.equals("Cardano") || cryptoChoice.equals("Ripple") || cryptoChoice.equals("Polkadot"))
                                                {
                                                    loggedUser.buy(cryptoChoice, cryptoQTY);
                                                    marketChoice = true;
                                                }
                                            }while(!marketChoice);
                                        break;

                                        case "Limit":
                                        //Bid for crypto
                                            System.out.print("-----------------------------------------\n");
                                            loggedUser.viewCryNames();
                                            System.out.print("Enter what crypto you'd like to buy as BIG: ");
                                            String cryptoChoice = input.nextLine();
                                            System.out.print("Enter the amount you'd like to buy: ");
                                            int cryptoQTY = input.nextInt();
                                            System.out.print("Enter the value at which you'd like to buy each coin");
                                            Double cryptoValue = input.nextDouble();

                                            if(cryptoChoice.equals("Bitcoin") || cryptoChoice.equals("Ethereum") || cryptoChoice.equals("Cardano") || cryptoChoice.equals("Ripple") || cryptoChoice.equals("Polkadot"))
	                                        {
                                                loggedUser.setBid(cryptoChoice, cryptoValue, cryptoQTY);
                                            }

                                        break;
                                    }
                                break;

                                case 3:
                                    //Sell Crypto
                                    boolean marSellValid = false;
                                    String marlimSellChoice;
                                    //First we buy the crypto
                                    do
                                    {
                                        System.out.print("\nEnter 'Market' Order or 'Limit' Order: ");
                                        marlimSellChoice = input.nextLine();

                                        if(marlimSellChoice.equals("Market"))
                                        {
                                            marSellValid = true;
                                        }
                                        else if(marlimSellChoice.equals("Limit"))
                                        {
                                            marSellValid = true;
                                        }
                                        else
                                        {
                                            System.out.println("Please enter a valid choice. \n");
                                        }
                                    }while(!marSellValid);

                                    switch(marlimSellChoice)
                                    {
                                        case "Market":
                                        boolean marketChoice = false;

                                        do
                                        {
                                            System.out.print("-----------------------------------------\n");
                                            loggedUser.viewCryNames();
                                            System.out.print("What crypto would you like to sell from the Market: ");
                                            String cryptoChoice = input.nextLine();
                                            System.out.print("Enter the amount of crypto you'd like to sell: ");
                                            int cryptoQTY = input.nextInt();

                                            if(cryptoChoice.equals("Bitcoin") || cryptoChoice.equals("Ethereum") || cryptoChoice.equals("Cardano") || cryptoChoice.equals("Ripple") || cryptoChoice.equals("Polkadot"))
	                                        {
                                                loggedUser.sellCrypto(cryptoChoice, cryptoQTY);
                                                marketChoice = true;
                                            }
                                        }while(!marketChoice);

                                    break;
                                    

                                        case "Limit"://Ask
                                            System.out.print("-----------------------------------------\n");
                                            loggedUser.viewCryNames();
                                            System.out.print("Enter what crypto you' like to sell as ASK: ");
                                            String cryptoChoice = input.nextLine();
                                            System.out.print("Enter the amount you wish to sell: ");
                                            int cryptoQTY = input.nextInt();
                                            System.out.print("Enter the value at which you'd like to sell each coin: ");
                                            Double cryptoValue = input.nextDouble();

                                            if(cryptoChoice.equals("Bitcoin") || cryptoChoice.equals("Ethereum") || cryptoChoice.equals("Cardano") || cryptoChoice.equals("Ripple") || cryptoChoice.equals("Polkadot"))
                                            {
                                                loggedUser.setAsk(cryptoChoice, cryptoValue, cryptoQTY);
                                            }
                                    break;
                                    }
                                break;

                                case 4:
                                    //To view the transactions
                                    System.out.println(loggedUser.getTransactions());
                                    System.out.println(loggedUser.getBalance()); //To display the user's current Balance

                                break;

                                case 5:
                                    boolean amnt = false;
                                    double deposit = 0.0;

                                    do
                                    {
                                        System.out.print("Enter Deposit Amount: ");

                                        try
                                        {
                                            String depamnt = input.nextLine();
                                            deposit = Double.parseDouble(depamnt.trim());
                                        }
                                        catch (NumberFormatException nfe) 
                                        {
                                            System.out.println(nfe.getMessage());
                                        }

                                        if(deposit <= 0)
                                        {
                                            System.out.println("Enter a valid amount.");
                                        }
                                        else
                                        {
                                            amnt = true;
                                            loggedUser.setBalance(deposit);
                                        }
                                    }while(!amnt);
                                break;

                                case 6:
                                    System.out.println("Your current Balance is $" + loggedUser.getBalance());
                                    System.out.println(loggedUser.getWallet());
                                break;

                                case 7:
                                    //if the user wants to log out
                                    loggedIn = User.logOut();
                                break;

                                default:
                                break;
                                }
                                
                        }while(loggedIn);
                    }
                }
                else
                {
                    Administrator admin = new Administrator();

                    do
                    {
                        choice = AdminMenu();

                        switch (choice)
                        {
                            case 1:
                                ArrayList<Trader> approved = admin.toApprove(registeredUser); //to approve any trader users that need approval

                                for (Trader user : approved) //loops through the Trader users that need approval
                                {
                                    registeredUser.remove(user.getUsername());
                                    users.put(user.getUsername(), user);
                                    System.out.println("User has been approved: " + user.getUsername());
                                }

                            break;

                            case 2: //To log out
                                System.out.println("Logging out of System");
                                loggedIn = User.logOut();
                            break;

                            default:
                            break;
                        }
                    }while(loggedIn);
                }

            }
        }
        input.close();
    }
}
