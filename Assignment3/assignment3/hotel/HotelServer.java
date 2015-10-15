import java.rmi.Naming;

public class HotelServer {
    public HotelServer(String host) {
        try {
            HotelInterface c = new HotelImpl();
            Naming.rebind("rmi://"+host+"/Hotel", c);
        } catch (Exception e) {
            System.out.println("Trouble: " + e);
        }
    }

    public static void main(String args[]) {
        new HotelServer(args[0]);
    }
}
