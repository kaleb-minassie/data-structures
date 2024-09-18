
/**
 * This class implements a circular linked list data structure. You need to 
 * implement this class. The Tank nodes are stored in the linked list which is 
 * represented by the reference to its current node stored in the member variable m_current. 
 * Every tank has a unique ID number. The list does not hold duplicate tank IDs. 
 * The tank IDs are integer numbers greater than or equal to zero.
 * @author Professor Kartchner
 *
 */
public class FuelSys {// UMBC - CMSC 341 - Spring 2023 - Proj1

	private Tank m_current;

	/**
	 * default minimum capacity of a tank
	 */
	public int MINCAP = 2000;
	
	/**
	 * default maximum capacity of a tank
	 */
	public int DEFCAP = 5000;
	
	/**
	 * default constructor for a FuelSys. Sets the current location (m_current) to null
	 */
	public FuelSys(){
	    //implement
		m_current = null;
	}
	
	/*~FuelSys(){
   	//not needed for Java implementation
	}*/
	
	/**
	 * The copy constructor creates a deep copy of a FuelSys. 
	 * Reminder: a deep copy means the new object has the same information 
	 * as the existing object, however, it has its own memory allocated. 
	 * Note: the new copy is an exact copy of existing object. For example, 
	 * the pumps of a tank in the new object appear in the same order as in 
	 * the existing object.
	 * @param rhs the FuelSys object that is being copied
	 */
	public FuelSys(FuelSys rhs) {
		//implement
		if (rhs != null){
			// creates a deep copy of the FuelSys
			if (rhs.m_current != null) {
				// copies the first tank
				this.m_current = new Tank(rhs.m_current);
				Tank sourceTank = rhs.m_current;
				Tank copyTank = this.m_current;

				//copying the rest of the tanks
				while (sourceTank.m_next != rhs.m_current) {
					sourceTank = sourceTank.m_next;
					copyTank.m_next = new Tank(sourceTank);
					copyTank = copyTank.m_next;
				}
				//to make it circular
				copyTank.m_next = this.m_current;
			}
		}
	}
	
	/**
	 * The function advance advances the current (m_current) to the next tank
	 * @return true if tank list is not empty, otherwise returns false
	 */
	public boolean advance() {// advance cursor (current)
		//implement
		if (m_current == null) {
			return false;
	}
	m_current = m_current.m_next;
			return true;
}
	
	/** The function getCurrentTank returns the current (m_current) for the tank list
	 *  @return m_current
	 */
	public Tank getCurrentTank() {
		//implement
		return m_current;
	}
	
	/**
	 * The function empty checks if the tank list is empty
	 * @return true if the list is empty, otherwise false
	 */
	public boolean empty()		// is list empty?
	{ 
		//implement
		if (m_current == null){
			return true; //list is empty
		} else {
		return false; //list is not empty
		}
	}

	/** The function addTank creates and inserts a new Tank object after the current 
	 *  location of the list and makes the node the current location. If the 
	 *  insertion is successful the function returns true, otherwise it returns 
	 *  false. Since the tank IDs must be unique in the list this function will 
	 *  not insert the tank if it already exists. In such a failure case the 
	 *  function returns false. The tankCap cannot be less than the min capacity. 
	 *  The amount of fuel cannot exceed the tank capacity. The tankID must be greater than 0.
	 *  @param tankID the ID for the tank
	 *  @param tankCap the capacity for the tank
	 *  @param tankFuel the amount of fuel currently being held
	 *  @return true if the insertion is successful, otherwise false
	 */
	public boolean addTank(int tankID, int tankCap, int tankFuel) {
	   //implement
	   if (findTank(tankID) || tankID <= 0 || tankCap < MINCAP || tankFuel > tankCap) {
	    return false;
	}

	/**
	 * The function remove removes the tank after the current tank in the tank list
	 * @return true if tank removed successfully, false if tank list is empty
	 */
	public boolean remove() {
	    //implement
		//check if the tank is empty
		if (m_current == null){
		return false;
	}
	//find the tank that precedes the current tank in the list
	Tank prevTank = getPreviousTank(m_current);
	
	//check if there's only one tank in the list
	if (prevTank == m_current) {
		m_current = null; // removing the last tank, list becomes empty
	} else {
		prevTank.m_next = m_current.m_next;
		m_current = prevTank.m_next;
	}
	return true;
}
	/** This function removes the tank with tankID from the list. 
	 *  If the requested tank is removed the function returns true
	 *  otherwise it returns false, i.e. if tankID does not exist 
	 *  the function returns false. Note, if the tank has pumps the 
	 *  list of pumps must be removed too. The current location remains unchanged.
	 *  @param tankID ID of the tank
	 *  @return true if the tank is removed, false if tank doesn't exist
	 */
	public boolean removeTank(int tankID){
	   //implement
	   if (m_current == null){
	    return false;
	   }
		Tank prevTank = getPreviousTank(tankID);
        if (prevTank == m_current && prevTank.m_next.getTankID() == tankID) {
            // removing the only tank in the list
            m_current = null;
            return true;
        } else if (prevTank.m_next.getTankID() == tankID) {
            // tank to remove is not the current tank
            prevTank.m_next = prevTank.m_next.m_next;
            return true;
        }
        return false;
	}
	
	/** The function findTank searches the list for tankID. If the ID is found 
	 * the function makes the current location the node before the found 
	 * tankID node and it returns true. If the ID is not found the 
	 * function returns false.
	 * @param tankID ID of the tank
	 * @return true if the tankID is found, otherwise false
	 */
	public boolean findTank(int tankID){
	    //implement
		if(m_current == null) {
	    return false;
	}
	Tank currentTank = m_current;
	do{
		currentTank = currentTank.m_next;
		if(currentTank.getTankID() == tankID){
			m_current = getPreviousTank(tankID);
			return true;
		}
	} while (currentTank != m_current);
	return false;
}
	
	/** This function finds the tankID and adds a pump with pumpID to the tank's 
	 * pump list. The targetTank is the tank ID that will receive fuel from the 
	 * parent of the pump in the case of transfer requests. Both tankID and targetID 
	 * must exist in the list in order to add the pump. In the case of success the 
	 * function returns true, otherwise it returns false. The pump must be added to 
	 * the head of the pump list. The current location remains unchanged.
	 * @param tankID the ID of the tank with the pump
	 * @param pumpID the ID of the pump
	 * @param targetTank the tank ID for the tank that is receiving from the pump
	 * @return true if the pump was added successfully, otherwise false
	 */
	public boolean addPump(int tankID, int pumpID, int targetTank){
	   //implement
	   if (m_current == null || m_current.getTankID() != tankID){
	    return false;
	}
	
	Tank currentTank = m_current;
	do{
		currentTank = currentTank.m_next;
		if (currentTank.getTankID() == targetTank){
			currentTank.addPump(pumpID, tankID);
			returns true;
		}
	} while (currentTank != m_current);
	return false;
}
	/** The function removePump removes the pumpID from the tankID. If the remove operation is successful 
	 *  the function returns true. If either tankID or pumpID does not exist it is a failure and the
     *  function returns false. The current location remains unchanged.
     *  @param tankID the ID of the tank
     *  @param pumpID the ID of the pump to be removed from the tank
     *  @return true if pumpID is removed from tankID, otherwise false
     */
	public boolean removePump(int tankID, int pumpID){
	    //implement
		if(m_current == null || m_current.getTankID() != tankID){
	    return false;
	}
	
	Tank currentTank = m_current;
	do{ 
		currentTank = currentTank.m_next;
		if(currentTank.getTankID() == tankID){
			return currentTank.removePump(pumpID);
		}
	} while (currentTank != m_current);
	return false;
}
	/**
	 * The function totalFuel calculates and returns the total amount of current fuel in the airplane. 
	 * The total amount of fuel is the sum of fuel in all tanks.
	 * @return total amount of fuel in all tanks
	 */
	public int totalFuel() {
	    //implement
		if (m_current == null){
	    return 0;
	}
	int totalFuel = 0;
        Tank currentTank = m_current;
        do {
            totalFuel += currentTank.getTankFuel();
            currentTank = currentTank.m_next;
        } while (currentTank != m_current);

        return totalFuel;
    }

	/**
	 * The function drain transfers the fuel from tankID to the targetID specified in the pumpID. 
	 * If the amount of requested transfer is more than the current empty space in the target 
	 * tank, the function fills the target tank and returns true. If the amount of requested 
	 * transfer is less than the empty space of the target tank the function transfers all and 
	 * returns true. If the amount of requested transfer is greater than the current fuel in the 
	 * source tank, the function performs the transfer and makes the source tank empty. 
	 * In a case of failure that either tank or the pump does not exist the function returns false.
	 * @param tankID the ID of the tank
	 * @param pumpID the ID of the pump within the tank
	 * @param fuel amount of fuel to move from tankID to target tank
	 * @return true if transfer is successful, otherwise false
	 */
	public boolean drain(int tankID, int pumpID, int fuel){
	    //implement
		if (m_current == null || m_current.getTankID() != tankID) {
	    return false;
	}
	Tank currentTank = m_current;
        do {
            currentTank = currentTank.m_next;
            if (currentTank.getTankID() == tankID) {
                return currentTank.drain(fuel, pumpID);
            }
        } while (currentTank != m_current);

        return false;
    }
	/**
	 * The function fill fills up the tankID with the amount of fuel. If the empty space 
	 * of the tank is less than fuel the function still fills up the tank up to its 
	 * capacity and returns true. If the tank does not exist the function returns false. 
	 * The current location remains unchanged.
	 * @param tankID the ID of the tank
	 * @param fuel the amount of fuel to add to the tank
	 * @return true if fill was successful, otherwise false
	 */
	public boolean fill(int tankID, int fuel){
	    //implement
		if (m_current == null || m_current.getTankID() != tankID) {
	    return false;
	}
	Tank currentTank = m_current;
        do {
            currentTank = currentTank.m_next;
            if (currentTank.getTankID() == tankID) {
                return currentTank.fill(fuel);
            }
        } while (currentTank != m_current);

        return false;
    }
	
	/**
	 * The function dumpSys returns the list of tanks along with the amount of current 
	 * fuel in the tank and the list of its pumps. The implementation of this 
	 * function is provided for the debugging purposes.
	 * @return String with list of tanks(amount of fuel) and list of it's pumps
	 */
	public String dumpSys() {
		String result = "";
	    if (m_current != null){
	        Tank tempTank = m_current.m_next;//we start at front item
	        //we continue visting nodes until we reach the cursor
	        while(m_current != null && tempTank != m_current){
	            result += "Tank " + tempTank.m_tankID + "(" + tempTank.m_tankFuel + " kg)\n";
	            // now dump the targets for all pumps in this tank
	            // we need to traverse the list of pumps
	            result += dumpPumps(tempTank.m_pumps);
	            tempTank = tempTank.m_next;
	        }
	        //at the end we visit the cursor (current)
	        //this also covers the case that there is only one item
	        result += "Tank " + m_current.m_tankID + "(" + m_current.m_tankFuel + " kg)\n";
	        dumpPumps(tempTank.m_pumps);
	        result += "The current tank is " + m_current.m_tankID + "\n";
	    }
	    else
	        result += "There is no tank in the system!\n\n";
	    return result;
	}

	/**
	 * The function dumpPumps returns the list of list of its pumps. The implementation of this 
	 * function is provided for the debugging purposes.
	 * @param pumps the head of the pump list
	 * @return String with the list of pumps for a tankID
	 */
	private String dumpPumps(Pump pumps) {
	    // this traverses the linked list to the end
		String result = "";
	    Pump tempPump = pumps;
	    while (tempPump != null){
	        result += " => pump " + tempPump.m_pumpID + "(To tank " + tempPump.m_target + ")\n";
	        tempPump = tempPump.m_next;
	    }
	    return result;
	}

	//Any additional private function you need go here
	
	/**
	 * The implementation of this class is provided. You are not allowed to modify this 
	 * class. The Tank class implements a node that can be used in a linked list. 
	 * Every node has a reference to the next node in the linked list. This reference is stored in 
	 * the m_next member variable. Every tank object has a unique ID number in this system. Moreover, 
	 * every tank can have multiple fuel pumps. The pumps of a tank are stored in a singly linked list (not circular) 
	 * accessible through the head of the list that is stored in the member variable m_pumps. The member variable 
	 * m_tankFuel stores the current amount of fuel in the tank. We can transfer part of the current fuel to 
	 * another tank using one of the pumps installed in the tank.
	 * @author Professor Kartchner
	 *
	 */
	class Tank{
		//Tank();
		Tank(int ID, int tankCap){
			m_tankID = ID; m_tankCapacity = tankCap;
			m_tankFuel = 0;
			m_pumps = null;
			m_next = null;
		}
		Tank(int ID, int tankCap, int tankFuel){
			m_tankID = ID; m_tankCapacity = tankCap;
			m_tankFuel = tankFuel;
			m_pumps = null;
			m_next = null;
		}
	    Tank(int ID, int tankCap, int tankFuel, 
	        Pump pumpList, Tank nextTank)
	    {
	        m_tankID = ID; m_tankCapacity = tankCap; m_tankFuel = tankFuel;
	        m_pumps = pumpList; m_next = nextTank;
	    }
	    
	    /**
	     * The function getTankID returns the tankID (m_tankID)
	     * @return m_tankID
	     */
	    int getTankID() {return m_tankID;}
	    
	    /**
	     * The function getTankCapacity returns the tankCapacity (m_tankCapacity)
	     * @return m_tankCapacity
	     */
	    int getTankCapacity() {return m_tankCapacity;}
	    
	    /**
	     * The function getTankFuel returns the tankFuel (m_tankFuel) which the 
	     * amount the tank is currently holding
	     * @return m_tankFuel
	     */
	    int getTankFuel() {return m_tankFuel;}
	    
	    /**
	     * The function getCurrentPump returns the current location (m_pumps) of the pump list
	     * @return m_pumps
	     */
	    Pump getCurrentPump() {return m_pumps;}
	    
	    private int m_tankID;
	    private int m_tankCapacity; // maximum capacity of the tank
	    private int m_tankFuel;     // current amount of fuel in the tank
	    private Pump  m_pumps;
	    private Tank  m_next;
		
	}
	
	/**
	 * The implementation of this class is provided. You are not allowed to modify this 
	 * class. The Pump class implements a node that can be used in a linked list. 
	 * Every node has a reference to the next node in the linked list. This reference is 
	 * stored in the m_next member variable. Every pump object has a unique ID number in 
	 * the context of its parent tank. Moreover, every pump has a target tank that can receive 
	 * fuel from the pump. This tank ID is stored in the member variable m_target. 
	 * @author Professor Kartchner
	 *
	 */
	class Pump{
	    //friend class FuelSys;
	    //Pump()
	    Pump(int ID, int target){
	        m_pumpID = ID; m_target = target;
	        m_next = null;
	    }
	    
	    Pump(int ID, int target, Pump nextPump){
	        m_pumpID = ID; m_target = target;
	        m_next = nextPump;
	    }
	    
	    /**
	     * The function getPumpID returns the ID of the pump (m_pumpID)
	     * @return m_pumpID
	     */
	    int getPumpID() {return m_pumpID;}
	    
	    /**
	     * The function getPumpTarget returns the ID of the target for this pump (m_pumpTarget)
	     * @return m_target
	     */
	    int getPumpTarget() {return m_target;}
	    
	    /**
	     * The function getPumpNext returns the ID of the pump next in the pump list (m_pumpID)
	     * @return m_next
	     */
	    Pump getPumpNext() {return m_next;}
	    
	    private int m_pumpID;
	    private int m_target; // ID of the target tank for transfer
	    private Pump m_next;
	};

}
