import java.util.Random;

class PrintThread2 extends Thread {
    private String message;
    private Boolean canWrite;
    public PrintThread2 otherPrint;
    private static String lock = new String("mylock");

    PrintThread2(String msg, Boolean cWrite) {
        message = msg;
        canWrite = cWrite;

    }

    public void run() {
        // synchronized(sync){
        //     while(sync == false)
        //     sync=true;
        // }
        for (int i = 0; i < 10 ; i++) {
            synchronized(lock) {
                while(!canWrite.booleanValue()) {
                    try {
                        lock.wait();
                    } catch(InterruptedException ex) {
                        System.out.println("Thread was interrupted!");
                    }
                }
                syn2.display(message);
                canWrite = new Boolean (false);
                otherPrint.canWrite = new Boolean(true);
                lock.notifyAll();
            }
        }
    }
}


public class syn2 {
    public static void main(String[] args) {
        int i = 0;

        Boolean writeAb =  new Boolean(true);
        Boolean  writeCd = new Boolean(false);
        Object ll = new Object();

        PrintThread2 t1 = new PrintThread2("ab", writeAb);
        PrintThread2 t2 = new PrintThread2("cd\n", writeCd);
        t1.otherPrint = t2;
        t2.otherPrint = t1;
        t1.start();
        t2.start();
    }

    public static void display(String str) {
        for(int i = 0; i < str.length(); i++) {
            System.out.print(str.charAt(i));
        }
    }
}