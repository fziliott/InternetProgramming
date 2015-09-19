class PrintMonitor {
    /*//condition value, when true the thread that print 'ab' has access, 
    when false the other thread can access*/
    private boolean writeAb = true;  

    synchronized public void printAb() throws InterruptedException {
        //if it is not my turn I wait
        while(!writeAb) {
            wait();
        }
        display("ab");
        writeAb = false;    //change the turn
        notifyAll();
    }
    synchronized public void printCd() throws InterruptedException {
        //if it is not my turn I wait
        while(writeAb) {
            wait();
        }
        display("cd\n");
        writeAb = true;     //change the turn
        notifyAll();
    }

    public static void display(String str) {
        for(int i = 0; i < str.length(); i++) {
            System.out.print(str.charAt(i));
        }
    }
}

class PrintThreadAb extends Thread {
    PrintMonitor monitor;
    PrintThreadAb(PrintMonitor m) {
        monitor = m;
    }

    public void run() {
        for (int i = 0; i < 10 ; i++) {
            try {
                monitor.printAb();
            } catch(InterruptedException ext) {
                System.out.println("PrintThreadAb was interrupted!");
                ext.printStackTrace();
            }
        }
    }
}

class PrintThreadCd extends Thread {
    PrintMonitor monitor;
    PrintThreadCd(PrintMonitor m) {
        monitor = m;
    }
    
    public void run() {
        for (int i = 0; i < 10 ; i++) {
            try {
                monitor.printCd();
            } catch(InterruptedException ext) {
                System.out.println("PrintThreadCd was interrupted!");
                ext.printStackTrace();
            }
        }
    }
}

public class syn2 {
    public static void main(String[] args) throws InterruptedException {
        PrintMonitor monitor = new PrintMonitor();      //monitor shared between 2 threads
        PrintThreadAb tAb = new PrintThreadAb(monitor);
        PrintThreadCd tCd = new PrintThreadCd(monitor);
        tAb.start();
        tCd.start();
    }
}