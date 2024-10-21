import java.util.*;

public class User {
    private String username;
    private String password;
    private UUID userId;

    public User()
    {

    }

    public User(String newUsername, String newPassword, UUID newUserId) 
    {
        this.username = newUsername;
        this.password = newPassword;
        this.userId = newUserId;
    }

    //Setting up the Setters
    public void setUsername(String newUsername)
    {
        this.username = newUsername;
    }

    public void setPassword(String newPassword)
    {
        this.password = newPassword;
    }

    public void setUserId(UUID newUserId)
    {
        this.userId = newUserId;
    }

    //Setting up the Getters
    public String getUsername()
    {
        return this.username;
    }

    public String getPassword()
    {
        return this.password;
    }

    public UUID getUserId()
    {
        return this.userId;
    }

    static boolean logOut()
    {
        return false;
    }
}