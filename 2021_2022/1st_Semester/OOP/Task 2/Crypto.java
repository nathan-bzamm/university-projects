import java.util.*;

public class Crypto 
{
    Map<String, Double> Cryptos = new HashMap<String, Double>();
    
    public Crypto()
    {
        Cryptos.put("Bitcoin", 37910.63);
        Cryptos.put("Ethereum", 2922.35);
        Cryptos.put("Cardano", 1.14);
        Cryptos.put("Ripple", 62.76);
        Cryptos.put("Polkadot", 27.97);
    }

    public void displayCryDetails()
    {
        String cryptoName = "Crypto Name |";
        String cryptoPrice = "Current Price";

        System.out.printf("%n %-15s %-15s %n ", cryptoName, cryptoPrice); //This is done to Align to the left
        System.out.println("------------------------------");
        Cryptos.forEach((k,v) -> {System.out.printf("%-15s %-15f %n", k, v);});
        System.out.println("------------------------------");
    }

    public void displayCryNames()
    {
        int i = 1;
        for (Map.Entry<String, Double> crypto : Cryptos.entrySet())
        {
            System.out.printf("%-5s %-15s %n", i++, crypto.getKey());
        }
    }

    public double getCryValue(String cryptoName)
    {
        return Cryptos.get(cryptoName);
    }
}
