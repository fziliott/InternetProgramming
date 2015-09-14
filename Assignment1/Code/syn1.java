class PrintThread extends Thread {
    private String message;
    private static Object lock = new Object();


    PrintThread(String msg) {
        message = msg;
    }

    public void run() {
        for (int i = 0; i < 10 ; i++) {
            synchronized(lock) {
                syn1.display(message);
            }
        }
    }
}


public class syn1 {
    public static void main(String[] args) {
        int i = 0;

        PrintThread t1 = new PrintThread("Hello_world\n");
        PrintThread t2 = new PrintThread("Bonjour_monde\n");

        t1.start();
        t2.start();
    }
    
    public static void display(String str) {
        for(int i = 0; i < str.length(); i++) {
            System.out.print(str.charAt(i));
        }
    }
}