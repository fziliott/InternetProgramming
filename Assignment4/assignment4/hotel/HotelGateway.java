import java.io.*;
import java.net.*;
import java.rmi.Naming;
import java.lang.Thread;

class ClientHandler implements Runnable {
    Socket clientSocket = null;
    public ClientHandler(Socket s) {
        clientSocket = s;
    }
    public void finalize() throws IOException {
        if(clientSocket != null) {
            clientSocket.close();
        }
    }
    public void run() {
        try {
            boolean exitSession = false;
            BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(clientSocket.getOutputStream()));
            String inputLine = "", outputLine = "";
            writer.write("hotelgw>");
            writer.flush();
            while ((inputLine = in.readLine()) != null && exitSession == false) {


                String[] arguments = inputLine.split(" ");

                switch(arguments[0]) {
                case "h":
                    //arg = g.getOptarg();
                    writer.write("List of available commands:\n");
                    writer.write("\t-b <t> \"guestname\": books a room to type 't' for guest 'gname'\n");
                    writer.write("\t-g: lists all the guests of the hotel\n");
                    writer.write("\t-l: list available rooms\n");
                    writer.flush();
                    break;

                case "l":
                    writer.write(HotelGateway.hotelServer.listRooms() + "\n");
                    writer.flush();

                    break;
                case "b":
                    //writer.write( "\n");
                    //writer.flush();
                    if( arguments.length > 2) {
                        int type = 42;
                        //writer.write(arguments[g.getOptind() ]);

                        //writer.flush();
                        try {
                            type = Integer.parseInt(arguments[1]);
                        } catch(NumberFormatException e) {}

                        String name = arguments[2];
                        int i=3;
                        while(i<arguments.length){
                            name=name + " " + arguments[i];
                            System.out.println(name);
                            i++;
                        }
                        if(HotelGateway.hotelServer.book(type, name)) {
                            writer.write("ok\n");
                        } else {
                            writer.write("NA\n");
                        }
                    } else {
                        writer.write("NA\n");

                    }
                    writer.flush();

                    break;
                case "g":
                    writer.write(HotelGateway.hotelServer.listGuests());
                    writer.flush();
                    break;
                case "q":
                    exitSession = true;
                    break;
                default:
                    break;
                }
                writer.write("hotelgw>");
                writer.flush();
            }
            if(clientSocket != null) {
                clientSocket.close();
            }
        } catch(Exception e) {}
    }
}

public class HotelGateway {
    public static HotelInterface hotelServer = null;
    static ServerSocket hotelSocket = null;

    public static void main(String args[]) throws Exception {
        boolean exitSession = false;
        int c;

        if(args.length > 0) {
            InetAddress addr = InetAddress.getByName(args[0]);
            hotelSocket = new ServerSocket(3211, 5, addr);
            hotelServer = (HotelInterface) Naming.lookup("rmi://" + args[0] + "/Hotel");

            while(true) {
                Socket clientSocket = hotelSocket.accept();
                (new Thread(new ClientHandler(clientSocket))).start();

            }
        }
    }
    public void finalize() throws IOException{
        if(hotelSocket != null) {
            hotelSocket.close();
        }
    }
}




