import java.util.Random;
import java.util.stream.IntStream;

public class driver {		    
	  
	public static void main(String[] args) {
	    if (addTankNormalTest())
	        System.out.println("addTankNormalTest() passed!");
	    else
	    	System.out.println("addTankNormalTest() failed!");
	    if (addTankErrorTest())
	    	System.out.println("addTankErrorTest() passed!");
	    else
	    	System.out.println("addTankErrorTest() failed!");
	}
	    
	public static boolean addTankNormalTest(){
			boolean result = true;
			// insert multiple tanks
			// check if it returns true
			// check if inserted at current
			FuelSys sys = new FuelSys();
			int numTanks = 20;
			int fuelArray[];
			Random randFuel = new Random(System.currentTimeMillis());
			//20 integers are created
			IntStream fuels = randFuel.ints(numTanks,sys.MINCAP, sys.DEFCAP);
			fuelArray = fuels.toArray();
			for (int i=0;i<numTanks&&result;i++){
				result = (sys.addTank(i,sys.DEFCAP,fuelArray[i]));
				result = (sys.getCurrentTank().getTankID() == i);
			}
			sys.dumpSys();
			return result;
		}

	public static boolean addTankErrorTest(){
			boolean result = true;
			// insert multiple tanks
			// must return false for duplicates
			// must return false for negative ID, and do not insert
			// must return false if capacity < fuel, and do not insert
			FuelSys sys = new FuelSys();
			int numTanks = 20;
			int fuelArray[];
			Random randFuel = new Random(System.currentTimeMillis());
			//20 integers are created
			IntStream fuels = randFuel.ints(numTanks,sys.MINCAP, sys.DEFCAP);
			fuelArray = fuels.toArray();
			for (int i=0;i<numTanks;i++){
				sys.addTank(i,sys.DEFCAP,fuelArray[i]);
				//try to add in a duplicate
				result = (sys.addTank(i,sys.DEFCAP,fuelArray[i]) == false) && result;
			}
			//try to add an invalid tankNumber, must >= 0
			result = (sys.addTank(-10, sys.DEFCAP,sys.DEFCAP) == false) && result;
			result = (sys.getCurrentTank().getTankID() != -10) && result;
			//try to add a tank with tankFuel > DEFCAP
			result = (sys.addTank(numTanks+1, sys.DEFCAP, sys.DEFCAP+1000) == false) && result;
			result = (sys.getCurrentTank().getTankID() != (numTanks+1)) && result;
			//try to add a tank with tankFuel < MINCAP
			result = (sys.addTank(numTanks+2, sys.MINCAP-500, sys.MINCAP-500) == false) && result;
			result = (sys.getCurrentTank().getTankID() != numTanks+2) && result;
			sys.dumpSys();
			return result;
	}
}
