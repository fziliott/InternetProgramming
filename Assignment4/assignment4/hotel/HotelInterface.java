import java.lang.String;

public interface HotelInterface extends java.rmi.Remote {
     boolean book(int roomType, String guest) throws java.rmi.RemoteException;
     String listRooms() throws java.rmi.RemoteException;
     String listGuests() throws java.rmi.RemoteException;
}
