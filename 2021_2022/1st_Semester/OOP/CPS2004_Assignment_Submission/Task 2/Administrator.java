import java.util.*;

public class Administrator extends User 
{
    public ArrayList<Trader> toApprove(Map<String, Trader> registeredUser)
    {
        Scanner input = new Scanner(System.in);

        String adminInput;
        boolean valid = false;

        ArrayList<Trader> approved = new ArrayList<Trader>();

        for (Map.Entry<String, Trader> user : registeredUser.entrySet())
        {
            valid = false;
            System.out.println(user.getKey() + " " + user.getValue());
            System.out.println("Approve User: Y/N");
            adminInput = input.nextLine();

            do
            {
                if(adminInput.equals("y"))
                {
                    approved.add(user.getValue());
                    System.out.println("User has been approved.");
                    valid = true;
                }
                else if(adminInput.equals("n"))
                {
                    System.out.println("User has not been approved.");
                    valid = true;
                }
                else
                {
                    System.out.println("Please enter Y or N");
                }
            }while(!valid);
        }

        return approved;
    }
}
