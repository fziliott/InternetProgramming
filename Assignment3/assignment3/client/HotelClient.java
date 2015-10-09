import java.net.MalformedURLException;
import java.rmi.Naming;
import gnu.getopt.Getopt;

public class HotelClient {
    public static void main(String[] args) {
        
        try {
            int c;
            String arg;

            Getopt g = new Getopt("HotelClient", args, "hl:b::g:");
            Hotel hotelServer = (Hotel) Naming.lookup("rmi://localhost/Hotel");

            while ((c = g.getopt()) != -1) {
                System.out.println(c);

                switch(c) {
                case 'h':
                    arg = g.getOptarg();
                    System.out.println("List of available commands:");
                    System.out.println("\t-l: list available rooms");
                    System.out.println("\t-b <t> \"gname\": books a room to type 't' for guest 'gname'");
                    break;

                case 'l':
                    System.out.println(g.getOptarg());
                    System.out.println(hotelServer.listRooms());
                    break;
                case 'b':
                    arg = g.getOptarg();
                    System.out.println(arg);
                    hotelServer.book(1,"ho");
                    break;
                case 'g':
                    arg = g.getOptarg();
                    System.out.println("You picked option '" + (char)c +
                                       "' with argument " +
                                       ((arg != null) ? arg : "null"));
                    break;
                default:
                    System.out.print("getopt() returned " + c + "\n");
                }
            }


            // Let's now try to divide by zero!
            // The server will continue to work.
            // The exception will be propagated to the client.
            //System.out.println( c.div(10, 0) ); // Division by zero!!
        } catch (Exception e) {
            System.out.println("Received Exception:");
            System.out.println(e);
        }
    }
}

