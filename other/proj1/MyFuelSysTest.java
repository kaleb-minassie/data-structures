import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class MyFuelSysTest {
FuelSys f1, f2, f3;
	
	@BeforeEach
	void setUp() throws Exception {
		f1 = new FuelSys();
		f2 = new FuelSys();
		//add 25 tanks to FuelSys
		// 25->24->23-> ... ->3->2->1->(back to 25), 1 is the current location
		for(int i=25; i>0; i--) {
			f2.addTank(i, f2.MINCAP+100*i, f2.MINCAP+50*i);
		}
		f3 = new FuelSys(f2);

	}

	@AfterEach
	void tearDown() throws Exception {
		f1 = f2 = f3 = null;
	}


	/**
	 * The copy constructor creates a deep copy of a FuelSys. 
	 * Reminder: a deep copy means the new object has the same information 
	 * as the existing object, however, it has its own memory allocated. 
	 * Note: the new copy is an exact copy of existing object. For example, 
	 * the pumps of a tank in the new object appear in the same order as in 
	 * the existing object.
	 */
	@Test
	void testCopyConstructor() {
		assertEquals(1,f3.getCurrentTank().getTankID());
		f3.advance();
		assertEquals(25,f3.getCurrentTank().getTankID());
		f3.advance();
		assertEquals(24,f3.getCurrentTank().getTankID());
	}

	/**
	 * This function advances the current (m_current) to the next tank
	 * @return true if tank list is not empty, otherwise returns false
	 */
	@Test
	void testAdvance() {
		assertEquals(1,f2.getCurrentTank().getTankID());
		assertTrue(f2.advance());
		assertEquals(25,f2.getCurrentTank().getTankID());
	}

	/** This function returns the current (m_current) for the tank list
	 *  @return m_current
	 */
	@Test
	void testGetCurrentTank() {
		assertEquals(null,f1.getCurrentTank());
		assertEquals(1,f2.getCurrentTank().getTankID());
	}

	/**
	 * The function checks if the tank list is empty
	 * @return true if the list is empty, otherwise false
	 */
	@Test
	void testEmpty() {
		fail("Not yet implemented");
	}

	/** This function creates and inserts a new Tank object after the current 
	 *  location of the list and makes the node the current location. If the 
	 *  insertion is successful the function returns true, otherwise it returns 
	 *  false. Since the tank IDs must be unique in the list this function will 
	 *  not insert the tank if it already exists. In such a failure case the 
	 *  function returns false. The tankCap cannot be less than the min capacity. 
	 *  The amount of fuel cannot exceed the tank capacity. The tankID must be greater than 0.
	 *  @return true if the insertion is successful, otherwise false
	 */
	@Test
	void testAddTank() {
		//parameters are legal for a tank
		assertTrue(f1.addTank(1, f1.MINCAP, f1.MINCAP));
		assertEquals(1, f1.getCurrentTank().getTankID());
		assertEquals(f1.MINCAP, f1.getCurrentTank().getTankCapacity());
		assertEquals(f1.MINCAP, f1.getCurrentTank().getTankFuel());
	}

	/**
	 * The function removes the tank after the current tank in the tank list
	 * @return true if tank removed successfully, false if tank list is empty
	 */
	@Test
	void testRemove() {
		fail("Not yet implemented");
	}

	/** This function removes the tank with tankID from the list. 
	 *  If the requested tank is removed the function returns true
	 *  otherwise it returns false, i.e. if tankID does not exist 
	 *  the function returns false. Note, if the tank has pumps the 
	 *  list of pumps must be removed too. The current location remains unchanged.
	 *  @return true if the tank is removed, false if tank doesn't exist
	 */
	@Test
	void testRemoveTank() {
		fail("Not yet implemented");
	}

	/** This function searches the list for tankID. If the ID is found 
	 * the function makes the current location the node before the found 
	 * tankID node and it returns true. If the ID is not found the 
	 * function returns false.
	 * @return true if the tankID is found, otherwise false
	 */
	@Test
	void testFindTank() {
		// f2 setup: 25->24->23-> ... ->3->2->1->(back to 25), 1 is the current location
		assertEquals(1,f2.getCurrentTank().getTankID());
		f2.advance();
		assertEquals(25,f2.getCurrentTank().getTankID());
		f2.advance();
		assertEquals(24,f2.getCurrentTank().getTankID());
		assertTrue(f2.findTank(2));
		//since 2 is a found tankID, move the current location to the node before, tankID = 3
		assertEquals(3,f2.getCurrentTank().getTankID());
	}

	/** This function finds the tankID and adds a pump with pumpID to the tank's 
	 * pump list. The targetTank is the tank ID that will receive fuel from the 
	 * parent of the pump in the case of transfer requests. Both tankID and targetID 
	 * must exist in the list in order to add the pump. In the case of success the 
	 * function returns true, otherwise it returns false. The pump must be added to 
	 * the head of the pump list. The current location remains unchanged.
	 * @return true if the pump was added successfully, otherwise false
	 */
	@Test
	void testAddPump() {
		assertTrue(f1.addTank(1, f1.MINCAP, f1.MINCAP));
		assertTrue(f1.addTank(2, f1.DEFCAP, f1.DEFCAP));
		assertTrue(f1.addTank(3, f1.DEFCAP, f1.DEFCAP));
		//head of list (first) tank is tankID 3
		assertEquals(3,f1.getCurrentTank().getTankID());
		//add pump 1 to tankID 1, receiving tank is tankID 2
		assertTrue(f1.addPump(1, 1, 2));
		//advance to tankID 1
		f1.advance();
		assertEquals(1,f1.getCurrentTank().getTankID());
		assertEquals(1,f1.getCurrentTank().getCurrentPump().getPumpID());
	}

	/** This function removes the pumpID from the tankID. If the remove operation is successful 
	 *  the function returns true. If either tankID or pumpID does not exist it is a failure and the
     *  function returns false. The current location remains unchanged.
     *  @return true if pumpID is removed from tankID, otherwise false
     */
	@Test
	void testRemovePump() {
		fail("Not yet implemented");
	}

	/**
	 * This function calculates and returns the total amount of current fuel in the airplane. 
	 * The total amount of fuel is the sum of fuel in all tanks.
	 * @return total amount of fuel in all tanks
	 */
	@Test
	void testTotalFuel() {
		//add 3 tanks
		assertTrue(f1.addTank(1, 3000, 2000));
		assertTrue(f1.addTank(2, 4000, 3000));
		assertTrue(f1.addTank(3, 4000, 3500));

		assertEquals(8500, f1.totalFuel());
	}

	/**
	 * This function transfers the fuel from tankID to the targetID specified in the pumpID. 
	 * If the amount of requested transfer is more than the current empty space in the target 
	 * tank, the function fills the target tank and returns true. If the amount of requested 
	 * transfer is less than the empty space of the target tank the function transfers all and 
	 * returns true. If the amount of requested transfer is greater than the current fuel in the 
	 * source tank, the function performs the transfer and makes the source tank empty. 
	 * In a case of failure that either tank or the pump does not exist the function returns false.
	 * scenario 1, the amount of the transfer is <= empty space in targetID 
	 * scenario 2, the amount of the transfer is > empty space in targetID 
	 * scenario 3, the amount of the transfer is > amount of fuel in tankId
	 * scenario 4, invalid pumpID for tankID
	 * scenario 5, invalid tankID
	 */
	@Test
	void testDrain() {
		fail("Not yet implemented");
	}

	/**
	 * This function fills up the tankID with the amount of fuel. If the empty space 
	 * of the tank is less than fuel the function still fills up the tank up to its 
	 * capacity and returns true. If the tank does not exist the function returns false. 
	 * The current location remains unchanged.
	 * scenario 1, the amount of the fill is <= empty space in tankID
	 * scenario 2, the amount of the fill is > empty space in tankID
	 * scenario 3, tankID 5 doesn't exist
	 */
	@Test
	void testFill() {
		fail("Not yet implemented");
	}

	/**
	 * This function returns the list of tanks along with the amount of current 
	 * fuel in the tank and the list of its pumps. The implementation of this 
	 * function is provided for the debugging purposes.
	 */
	@Test
	void testDumpSys() {
		//add 3 tanks
				assertTrue(f1.addTank(1, 3000, 2000));
				assertTrue(f1.addTank(2, 5000, 3000));
				assertTrue(f1.addTank(3, 4000, 3500));
				assertEquals(8500, f1.totalFuel());
				//current tankID 3 holds 3500 gallons
				assertEquals(3500, f1.getCurrentTank().getTankFuel());
				//add 2 pumps to tankID 1
				f1.addPump(1, 1, 2);
				f1.addPump(1, 2, 3);
				String result = f1.dumpSys();
				String[] lines = result.split("\n");
				assertEquals("Tank 1(2000 kg)",lines[0].trim());
				assertEquals("=> pump 2(To tank 3)",lines[1].trim());
				assertEquals("=> pump 1(To tank 2)",lines[2].trim());
				assertEquals("Tank 2(3000 kg)",lines[3].trim());
				assertEquals("Tank 3(3500 kg)",lines[4].trim());
				assertEquals("The current tank is 3",lines[5].trim());
	}

}

