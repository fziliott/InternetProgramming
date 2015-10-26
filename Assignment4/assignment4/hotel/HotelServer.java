import java.rmi.Naming;

public class HotelServer {
    public HotelServer(String host) {
        try {
            HotelInterface c = new HotelImpl();
            Naming.rebind("rmi://" + host + "/Hotel", c);
        } catch (Exception e) {
            System.out.println("Trouble: " + e);
        }
    }

    public static void main(String args[]) {
        if(args.length == 1) {
            new HotelServer(args[0]);
        } else {
            new HotelServer("localhost");
        }
    }
}
