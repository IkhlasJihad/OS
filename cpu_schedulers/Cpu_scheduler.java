
import java.util.ArrayDeque;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Scanner;

/**
 * @author IkhlasJihad <ikhlasjaydi@gmail.com> 
 */
public class Cpu_scheduler {
    static Scanner input;
    private static String [] schedulers = {"" , "First Come, First Served (FCFS)", 
                       "Shorstest Job First (SJF)","Round Robin (RR)"};
    static int curSched, jobsCount, q;
    static Process[] jobs;
    static boolean firstTime = true;
    static PriorityQueue<Process> sjf ;
    static Queue<Process> roundRobin; 
    public static void main(String[] args) throws Exception {
        input = new Scanner(System.in);
        System.out.println("This Simple program shows the basic 3 cpu schedulers, "
                + "\nYou can choose which one to run :) "
                + "\nJust Enter the number of it in the given list below .. ");
        start();
        firstTime = false;
        while(curSched != 0){
            switch(curSched){
                case 1 : fcfs(); break;
                case 2: sjf();break;
                case 3: rr(); break;
                default: System.out.println("Invalid number .. try again a valid one or enter 0 to exit..");break;    
            }
            start();
        }
        if(curSched == 0)
            exit();
    }
    private static void start() throws Exception{    
        System.out.println("\n---------------- Schedulers List ----------------");
        for (int i = 1 ; i <= 3; i++){
            System.out.println("\t" + i +". " + schedulers[i]);
        }
        System.out.print("\n>>>>schedular No.: ");
        curSched = input.nextInt();
        if(curSched == 0) 
            exit();
        System.out.println("OK, You have chosen \'" + schedulers[curSched] + "\'");        
        if(!firstTime){
            System.out.println("Continue with the same list of jobs? { Y | N } :");
            String get = input.next().toUpperCase();
            switch(get){
                case "Y": fillQueue();return ;
                case "N": ;break;
                default : System.exit(1);
            }
        }
        System.out.print(">>>>Number of jobs: ");
        jobsCount = input.nextInt();
        jobs = new Process[jobsCount+1];
        System.out.print(">>>>Enter the length of each job in single space seperated line: ");
        for(int i = 1; i <= jobsCount; i++){
            jobs[i] = new Process(i, input.nextInt());
        }
        System.out.print(">>>>Specify the time slice: ");
        q = input.nextInt();
        fillQueue();        
    }
    private static void fillQueue(){
        sjf = new PriorityQueue<>();
        roundRobin = new ArrayDeque<>();
        for(int i = 1; i <= jobsCount ; i++){
            jobs[i].setRt(0); jobs[i].setWt(0); jobs[i].setTt(0); jobs[i].setRemain(jobs[i].getLength());
            sjf.add(jobs[i]);
            roundRobin.add(jobs[i]); 
        }
    }
    static void fcfs(){
        int total = 0 ;
        System.out.println("Start running ..");
        for(int i = 1; i <= jobsCount; i++){
            Process cur = jobs[i];
            jobs[i].setRt(total);
            jobs[i].setWt(total);
            total += cur.getLength();
            jobs[i].setTt(total);
            int curTime = (int)System.currentTimeMillis() % 10 ;
            for(int j = curTime; j < curTime + cur.getLength(); j++){
            }
            System.out.println("Yes, job -" + (cur.getNumber()) + "- of length " + cur.getLength() +" is done .." );
        }
        getStatistics();
    }
    private static void sjf(){
        System.out.println("Start running ..");
        int turntimeSJF = 0;
        while(!sjf.isEmpty()){
            Process p =  sjf.poll();
            // determine waitTime
            jobs[p.getNumber()].setWt(turntimeSJF);
            // determine ResponseTime
            jobs[p.getNumber()].setRt(turntimeSJF);
            // accumlate turnaroundTime 
            turntimeSJF += p.getLength();
            jobs[p.getNumber()].setTt(turntimeSJF);
            int curTime = (int)System.currentTimeMillis() % 10 ;
            for(int i = curTime; i < curTime + p.getLength(); i++){
            }
            System.out.println("Yes, job -" + (p.getNumber()) +
                    "- of length " + p.getLength() +" is done .." );
        }        
        getStatistics();
    }
    private static void rr(){
        System.out.println("Start running ..");
        int total = 0;
        while(!roundRobin.isEmpty()){
            Process p = roundRobin.poll();
            int runFor = (p.getRemaining() > q ? q : p.getRemaining()); // will be added to all other jobs
            if( p.getRemaining() == p.getLength()) {
                jobs[p.getNumber()].setRt(total);
            }
            p.setRemain(p.getRemaining()-q);
            total += runFor;       
            System.out.println("job" + p.getNumber() + " run for " + runFor +"ms" );
            if(p.getRemaining() > 0)
                roundRobin.add(p);
            else{
                jobs[p.getNumber()].setWt(total-p.getLength());
                jobs[p.getNumber()].setTt(total);
            }
        }
        getStatistics();
    }
    private static void getStatistics(){       
        System.out.print("Done Successfully\nDo You need to get the average time metrics ?{ Y | N }:");
        String get = input.next().toUpperCase();
        switch(get){
            case "Y": System.out.println("... Calculating ... "); break;
            case "N":
            default : return;
        }
        double sumT = 0, sumW = 0, sumR = 0;        
        System.out.println("... Preparing Results ... \n Be Careful >> Jobs are numbered according to their execution ");
        System.out.printf("%15s%15s%15s%15s\n"," ","Turnaround Time","Wait Time","Response Time");
        for(int i = 1; i <= jobsCount ; i++){
            Process p = jobs[i];
            System.out.printf(p.toString());
            sumT +=  p.getTt();
            sumW += p.getWt();
            sumR += p.getRt();
        }       
        displayAvaerages(sumT, sumW, sumR);
    }
    private static void displayAvaerages(double sumTt, double sumWt, double sumRt){
        String format = "%25s: %4.2f\n";
        System.out.printf(format, "Average Turnaround Time",sumTt/jobsCount);
        System.out.printf(format, "Average Wait Time",sumWt/jobsCount);
        System.out.printf(format, "Average Response Time",sumRt/jobsCount);
    }
    static void exit(){
        System.out.println("End of cpu_scheduler");
        System.exit(0);
    }

}
class Process implements Comparable<Process>{
    int num, length, remain, tt,wt,rt;
    public Process(int n, int l){
        num = n;
        length = l;
        remain = length;
    }
    int getNumber(){return num;}
    int getLength(){return length;}
    int getRemaining(){return remain;}

    public void setTt(int tt) {
        this.tt = tt;
    }

    public void setWt(int wt) {
        this.wt = wt;
    }

    public void setRt(int rt) {
        this.rt = rt;
    }

    public int getTt() {
        return tt;
    }

    public int getRt() {
        return rt;
    }
    
    public int getWt() {
        return wt;
    }
    void setRemain(int r){remain = r;}
    @Override
    public String toString(){
        return String.format("\tJob %3d | %15d%15d%15d\n", num, tt,wt,rt);
    
    }
    
    @Override
    public int compareTo(Process p){
        return length - p.length;
    }
}
