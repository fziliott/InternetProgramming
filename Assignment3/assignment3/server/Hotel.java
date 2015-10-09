import java.lang.String;

public interface Hotel extends java.rmi.Remote {
     boolean book(int roomType, String guest);
     String listRooms();
}
