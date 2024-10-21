public class Orderbook 
{
    String crypName;
    Double price;
    int quantity;
    String bid;

    public Orderbook(String cryptoName, Double cryptoPrice, int quantity, String cryptoBid)
    {
        this.crypName = cryptoName;
        this.price = cryptoPrice;
        this.quantity = quantity;
        this.bid = cryptoBid;
    }

    //Setting up Setters

    public void setCrypName(String cryptoName)
    {
        this.crypName = cryptoName;
    }

    public void setPrice(Double cryptoPrice)
    {
        this.price = cryptoPrice;
    }

    public void setQuantity(int cryptoQuantity)
    {
        this.quantity = cryptoQuantity;
    }

    public void setBid(String cryptoBid)
    {
        this.bid = cryptoBid;
    }

    //Setting up the Getters
    public String getCryptoName()
    {
        return this.crypName;
    }

    public double getPrice()
    {
        return this.price;
    }

    public int getQuantity()
    {
        return this.quantity;
    }

    public String getBid()
    {
        return this.bid;
    }

    public void getDetails(){
        System.out.printf("%-15s %-15s %-15s %15s %n", getCryptoName(), getPrice(), getQuantity(), getBid());
    }
}
