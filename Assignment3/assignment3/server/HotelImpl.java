public class HotelImpl implements Hotel{
	Rooms type1 = new Rooms (150, 10);
        Rooms type2 = new Rooms(120, 20);
        Rooms type3 = new Rooms(100, 20);
        Rooms[] rooms={type1, type2, type3};
 
        public synchronized String listRooms(){
        	return type1.available + "\t" + type2.available + "\t" + type3.available;
        }
        public synchronized boolean book(int type, String gname){
        	if( rooms[type].book()){
        		return true;
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