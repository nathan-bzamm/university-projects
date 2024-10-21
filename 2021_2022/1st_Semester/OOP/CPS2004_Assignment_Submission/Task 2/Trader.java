import java.util.*;

public class Trader extends User 
{
    private double balance;
    private ArrayList<String> transactions = new ArrayList<>();
    private Map<String, Integer> currency = new HashMap<String, Integer>();

    static ArrayList<Orderbook> orderBook = new ArrayList<>();
    static ArrayList<Orderbook> obBids = new ArrayList<>();
    static ArrayList<Orderbook> obAsks = new ArrayList<>();

    static Crypto c = new Crypto();

    public Trader()
    {

    }

    public Trader(String username, String password, UUID userId, double initBalance, ArrayList<String> userLog, Map<String, Integer> cryptoWallet)
    {
        super(username, password, userId);
        this.balance = initBalance;
        this.transactions = userLog;
        this.currency = cryptoWallet;
    }

    //Setting up the setters

    public void setBalance(double deposit)
    {
        this.balance += deposit;
    }

    public void setTransactions(ArrayList<String> userLog)
    {
        this.transactions = userLog;
    }

    public void setWallet(Map<String, Integer> cryptoWallet)
    {
        this.currency = cryptoWallet;
    }

    //setting up the getters

    public double getBalance()
    {
        return this.balance;
    }

    public ArrayList<String> getTransactions()
    {
        return this.transactions;
    }

    public Map<String, Integer> getWallet()
    {
        return this.currency;
    }

    //To View Crypto Details

    public void viewCryDetails()
    {
        c.displayCryDetails();
    }

    public void viewCryNames()
    {
        c.displayCryNames();
    }

    //Transactions

    public void displayTransactions()
    {
        if(this.transactions.isEmpty())
        {
            System.out.println("Error, Transaction Log is empty");
        }
        else
        {
            for(String transactionlog : this.transactions){
                System.out.println(transactionlog);
            }
        }
    }

    public void createTransaction(String cryptoName, double cryptoValue, boolean payType, boolean paymentSuccessFail)
    {
        String deal;
        String decision; //To tell if to buy or to sell

        if(paymentSuccessFail == true)
        {
            deal = "User successfully managed to ";
        }
        else
        {
            deal = "User has failed to ";
        }

        if(payType == true)
        {
            decision = "buy";
        }
        else
        {
            decision = "sell";
        }

        String cryptoTransaction = deal + decision + " " + cryptoName + "@" + cryptoValue + "/usd";
        this.transactions.add(cryptoTransaction);
    }

    //Function to Buy Crypto

    public void buy(String cryptoName, int quantity)
    {
        int remaining = quantity;
        double curBalance = this.balance;
        int cryptoQuantity = 0;

        for (Orderbook orderbook : obAsks)
        {
            int bought = 0;

            if(cryptoName.equals(orderbook.getCryptoName())) //confirm the crypto names
            {
                cryptoQuantity = orderbook.getQuantity();

                for(int i = 1; i <= cryptoQuantity; i++) //will loop through the quantity
                {
                    if(bought < quantity)
                    {
                        if(curBalance >= orderbook.getPrice()) //checks if the current balance is less than the asking price
                        {
                            curBalance -= orderbook.getPrice(); //The current balance is then adjusted, checked to see if its nor empty, will display if this is fulfilled or not
                            bought = i; //indicates the much the user succeeded to buy.
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                cryptoQuantity -= bought;
                remaining -= bought;
                //Remobing the order book ask if
                if(cryptoQuantity == 0)
                {
                    obAsks.remove(orderbook);
                }
                else
                {
                    orderbook.setQuantity(cryptoQuantity);
                }

                //creating a transaction for the amount of crypto bought at a price
                createTransaction(cryptoName, (orderbook.getPrice() * bought), true, true);

                if(this.currency.size() == 0)
                {
                    this.currency.put(cryptoName, bought);
                }
                else
                {
                    for (Map.Entry<String, Integer> w: this.currency.entrySet())
                    {
                        if(cryptoName.equals(w.getKey()))
                        {
                            this.currency.put(cryptoName, w.getValue() + bought);
                        }
                        else
                        {
                            this.currency.put(cryptoName, bought);
                        }
                    }
                }

                if(remaining == 0)
                {
                    System.out.println("Order has been fullfilled.");
                    break;
                }
            }
        }

        if(remaining > 0)
        {
            System.out.println("Order has not been fully fulfilled.");
        }
    }

    public void sellCrypto(String cryptoName, int quantity)
    {
        int remaining = quantity;
        double curBalance = this.balance;
        int cryptoQuantity = 0;

        for (Orderbook orderbook : obBids)
        {
            int sold = 0;

            if(cryptoName.equals(orderbook.getCryptoName())) //Confirm the name of the crypto that one might want to sell
            {
                cryptoQuantity = orderbook.getQuantity();
                for(int i = 1; i <= cryptoQuantity; i++) //Will loop through the amount of crypto
                {
                    if(curBalance >= orderbook.getPrice()) //Checks if the current balance is lower than the asking price
                    {
                        curBalance -= orderbook.getPrice(); //Current balance is then adjusted accordingly.  Will indicate if this is successfull or not.
                        sold = i; //Indicates how much the user successfully sold.
                    }
                    else
                    {
                        System.out.println("User does not have enough money");
                        break;
                    }
                }
                
                cryptoQuantity -= sold;
                remaining -= sold;

                //Removing the Order Book Bid if
                if(cryptoQuantity == 0)
                {
                    obBids.remove(orderbook);
                }
                else
                {
                    orderbook.setQuantity(cryptoQuantity);
                }

                //Creating a transaction for the amount of Crypto he bought at a price
                
                createTransaction(cryptoName, (orderbook.getPrice() * sold), true, true);

                for (Map.Entry<String, Integer> w: currency.entrySet())
                {
                    if(cryptoName.equals(w.getKey()))
                    {
                        currency.put(cryptoName, w.getValue() - sold);

                        if(remaining == 0)
                        {
                            currency.remove(w.getKey());
                        }
                    }
                }

                if(remaining == 0)
                {
                    System.out.println("Order has been fulfilled");
                    break;
                }
            }
        }
    }
    
    //To get a specific price

    public Double getPrice(String cryptoName)
    {
        return c.getCryValue(cryptoName);
    }

    //Limit order bid and ask

    public void setBid(String cryptoName, double cryptoPrice, int cryptoQuantity)
    {
        Orderbook ob =  new Orderbook(cryptoName, cryptoPrice, cryptoQuantity, "Bid");
        orderBook.add(ob);
        obBids.add(ob);
    }

    public void setAsk(String cryptoName, double cryptoPrice, int cryptoQuantity)
    {
        for (Map.Entry<String, Integer> w: currency.entrySet())
        {
            if(cryptoName.equals(w.getKey()))
            {
                if(w.getValue() <= 0)
                {
                    System.out.println("Not enough coins");
                }
                else
                {
                    Orderbook ob = new Orderbook(cryptoName, cryptoPrice, cryptoQuantity, "Ask");
                    orderBook.add(ob);
                    obAsks.add(ob);
                }
            }
            else
            {
                System.out.println("You do not have " + cryptoName + "in your account.");
            }
        }
    }

    public ArrayList<Orderbook> getAsks()
    {
        return obAsks;
    }

    public ArrayList<Orderbook> getBids()
    {
        return obBids;
    }

    public void initOrderBook()
    {
        Orderbook oba0 = new Orderbook("Bitcoin", 37910.63, 50000,"Ask");
        Orderbook oba1 = new Orderbook("Ethereum", 2922.35, 25000,"Ask");
        Orderbook oba2 = new Orderbook("Cardano", 1.14, 94328,"Ask");
        Orderbook oba3 = new Orderbook("Ripple", 62.76, 120000,"Ask");
        Orderbook oba4 = new Orderbook("Polkadot", 27.97, 106785,"Ask");

        //Adding to Asks Orderbook
        obAsks.add(oba0);
        obAsks.add(oba1);
        obAsks.add(oba2);
        obAsks.add(oba3);
        obAsks.add(oba4);

        Orderbook obb0 = new Orderbook("Bitcoin", 37910.63, 50000,"Bid");
        Orderbook obb1 = new Orderbook("Ethereum", 2922.35,25000,"Bid");
        Orderbook obb2 = new Orderbook("Cardano", 1.14, 94328,"Bid");
        Orderbook obb3 = new Orderbook("Ripple", 62.76,120000,"Bid");
        Orderbook obb4 = new Orderbook("Polkadot", 27.97, 106785,"Bid");

        //Adding to Bids Orderbook
        obBids.add(obb0);
        obBids.add(obb1);
        obBids.add(obb2);
        obBids.add(obb3);
        obBids.add(obb4);

        //Adding to the Orderbook

        orderBook.add(oba0);
        orderBook.add(oba1);
        orderBook.add(oba2);
        orderBook.add(oba3);
        orderBook.add(oba4);
        orderBook.add(obb0);
        orderBook.add(obb1);
        orderBook.add(obb2);
        orderBook.add(obb3);
        orderBook.add(obb4);

    }

    //To display the Orderbook
    public void displayOrderBook()
    {
        if(orderBook.size() == 0)
        {
            System.out.println("Orderbook is empty. \n");
        }
        else
        {
            System.out.printf("%n%-15s%-15s%20s%-15s%n", "Crypto Name", "Crypto Price", "Crypto Quantity", "Bid or Ask");
            for (Orderbook ob : orderBook) {
                ob.getDetails();
            }
        }
    }
}

