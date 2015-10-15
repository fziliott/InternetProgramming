import java.lang.String;
import java.util.ArrayList;

public class HotelImpl extends java.rmi.server.UnicastRemoteObject implements HotelInterface {
    ArrayList<String> guests = new ArrayList<String>();
    Rooms type1 = new Rooms (150, 10);
    Rooms type2 = new Rooms(120, 20);
    Rooms type3 = new Rooms(100, 20);
    Rooms[] rooms = {type1, type2, type3};

    public HotelImpl() throws java.rmi.RemoteException {
        super();
    }

    public String listGuests() throws java.rmi.RemoteException {
        String list = "";
        for(String s : guests) {
            list = list + s + "\n";
        }
        return list;
    }

    public String listRooms() throws java.rmi.RemoteException {
        return type1.available + "\t" + type2.available + "\t" + type3.available;
    }
    public synchronized boolean book(int type, String gname) {
        if(type>0 && type <= 3 ) {
            if(rooms[type-1].book()) {
                guests.add(gname);
                return true;
            }
        }
        return false;
    }
}

class Rooms {
    public Rooms(int p, int a) {
        price = p;
        available = a;
    }
    int price;
    int available;

    boolean book() {
        if(available > 0) {
            available--;
            return true;
        } else
            return false;
    }
}