import java.net.MalformedURLException;
import java.rmi.Naming;
import java.lang.Integer;
import gnu.getopt.Getopt;

public class HotelClient {
    public static void main(String[] args) {

        try {
            int c;
            String arg;

            Getopt g = new Getopt("HotelClient", args, "hlb::g");
            Hotel hotelServer = (Hotel) Naming.lookup("rmi://" + args[0] + "/Hotel");

            while ((c = g.getopt()) != -1) {
                switch(c) {
                case 'h':
                    arg = g.getOptarg();
                    System.out.println("List of available commands:");
                    System.out.println("\t-l: list available rooms");
                    System.out.println("\t-b <t> \"guestname\": books a room to type 't' for guest 'gname'");
                    break;

                case 'l':
                    //System.out.println(g.getOptarg());
                    System.out.println(hotelServer.listRooms());
                    break;
                case 'b':
                    if( g.getOptind() + 1 <= args.length) {
                        int type = 34;
                        try {
                            type = Integer.parseInt(args[g.getOptind()]);

                        } catch(NumberFormatException e) {}
                        String name = args[g.getOptind() + 1];


                        if(hotelServer.book(type, name)) {
                            System.out.println("ok");
                        } else {
                            System.out.println("NA");
                        }
                    } else {
                        System.out.println("NA");
                    }

                    break;
                case 'g':
                    System.out.println(hotelServer.listGuests());
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

