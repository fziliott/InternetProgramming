import java.net.MalformedURLException;
import java.rmi.Naming;
import java.lang.Integer;
import gnu.getopt.Getopt;

public class HotelClient {
    public static void main(String[] args) {

        try {
            int c;
            String arg;

            HotelInterface hotelServer = (HotelInterface) Naming.lookup("rmi://" + args[0] + "/Hotel");
            
            Getopt g = new Getopt("HotelClient", args, "hlb::g");

            while ((c = g.getopt()) != -1) {
                switch(c) {
                case 'h':
                    arg = g.getOptarg();
                    System.out.println("List of available commands:");
                    System.out.println("\t-b <t> \"guestname\": books a room to type 't' for guest 'gname'");
                    System.out.println("\t-g: lists all the guests of the hotel\n");
                    System.out.println("\t-l: list available rooms");
                    break;

                case 'l':
                    //System.out.println(g.getOptarg());
                    System.out.println(hotelServer.listRooms());
                    break;
                case 'b':
                    if(g.getOptind() + 1 <= args.length) {
                        int type = 42;
                        try {
                            type = Integer.parseInt(args[g.getOptind()]);

                        } catch(NumberFormatException e) {
                            System.out.println(e);
                        }
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
                    System.out.print(hotelServer.listGuests());
                    break;
                default:
                    System.out.print("Error parsing arguments, see the help with -h\n");
                }
            }

        } catch (Exception e) {
            System.out.println("Received Exception:");
            System.out.println(e);
        }
    }
}

