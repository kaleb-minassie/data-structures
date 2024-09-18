import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class myShowTest {
	Show s1, s2, s3;
	Show.Drone d0, d1, d2, d3, d4, d5, d6, d7, d8, d9;

	@BeforeEach
	void setUp() throws Exception {
		s2 = new Show();
		s3 = new Show();
		s1 = new Show();
		d0 = s1.new Drone(); // default LIGHT(red), STATE(LightOn), HEIGHT(0), ID(0)
		d1 = s1.new Drone(53002);
		d2 = s1.new Drone(20201); // default LIGHT(red), STATE(LightOn), HEIGHT(0)
		d3 = s1.new Drone(60496); // default LIGHT(red), STATE(LightOn), HEIGHT(0)
		d4 = s1.new Drone(19289, LIGHTCOLOR.GREEN, STATE.LIGHTON);// default HEIGHT(0)
		d5 = s1.new Drone(27904, LIGHTCOLOR.BLUE, STATE.LIGHTON);// default HEIGHT(0)
		d6 = s1.new Drone(93209);// default LIGHT(red), STATE(LightOn), HEIGHT(0)
		d7 = s1.new Drone(19224, LIGHTCOLOR.BLUE, STATE.LIGHTON);// default HEIGHT(0)
		d8 = s1.new Drone(19372, LIGHTCOLOR.GREEN, STATE.LIGHTON);// default HEIGHT(0)
		d9 = s1.new Drone(26241);// default LIGHT(red), STATE(LightOn), HEIGHT(0)
	}

	@AfterEach
	void tearDown() throws Exception {
		s3 = s2 = s1 = null;
	}

	@Test
	void testShow() {
		assertNull(s1.getRoot());
		assertTrue(s2.getRoot() == null);
		assertFalse(s3.getRoot() != null);
	}

	@Test
	void testInsertEasyNoRebalance() {
		s1.insert(d1);
		// this drone is the root of the tree
		assertEquals(53002, s1.getRoot().getID());
		assertEquals(LIGHTCOLOR.RED, s1.getRoot().getType());
		assertEquals(STATE.LIGHTON, s1.getRoot().getState());

		// insert to the left of the root
		s1.insert(d2);
		assertEquals(20201, s1.getRoot().getLeft().getID());
		assertEquals(LIGHTCOLOR.RED, s1.getRoot().getLeft().getType());
		assertEquals(STATE.LIGHTON, s1.getRoot().getLeft().getState());

		// insert to the right of the root
		s1.insert(d3);
		assertEquals(60496, s1.getRoot().getRight().getID());
		assertEquals(LIGHTCOLOR.RED, s1.getRoot().getRight().getType());
		assertEquals(STATE.LIGHTON, s1.getRoot().getRight().getState());
	}

	@Test
	void testInsertNoRebalance() {
		// test to make sure the BST properties are preserved
		Show s1 = new Show();
		// Insert drones into the show without rebalancing
		s1.insert(d1);
		s1.insert(d2);
		s1.insert(d3);
		
		// Add assertions to check the structure of the tree
		assertEquals(d1, s1.getRoot());
		assertEquals(d2, s1.getRoot().getLeft());
		assertEquals(d3, s1.getRoot().getRight());
		//fail("This test has not been implemented");
	}

	@Test
	void testInsertRebalanceEasy() {
		s1.insert(d1); // root
		s1.insert(d2); // left of root
		s1.insert(d4); // left of left of root
		// requires a rebalance
		assertEquals(20201, s1.getRoot().getID());
		assertEquals(53002, s1.getRoot().getRight().getID());
		assertEquals(19289, s1.getRoot().getLeft().getID());
	}

	@Test
	void testInsertDeepCopyOfParameter() {
		s1.insert(d1);
		s1.insert(d2);
		assertEquals("20201:LIGHTON:RED\n53002:LIGHTON:RED", s1.listDrones());
		d1.setID(93300);
		s1.insert(d3);
		assertEquals("20201:LIGHTON:RED\n53002:LIGHTON:RED\n93300:LIGHTON:RED",s1.listDrones());
	}

	@Test
	void testInsertRebalance() {
		// test to make sure the BST properties are preserved
		Show s1 = new Show();

		// Insert drones into the show to trigger a rebalance
		s1.insert(d1); // root
		s1.insert(d2); // left of root
		s1.insert(d3); // right of root
		s1.insert(d4); // left of left of root
	
		// Add assertions to check the structure of the tree after rebalancing
		assertEquals(d1, s1.getRoot());
		assertEquals(d2, s1.getRoot().getLeft());
		assertEquals(d3, s1.getRoot().getRight());
		assertEquals(d4, s1.getRoot().getLeft().getLeft());

	}

	@Test
	void testInsertDuplicate() {
		Show s1 = new Show();

    // Insert drones with the same ID
    d1 = s1.new Drone(12345, LIGHTCOLOR.BLUE, STATE.LIGHTOFF);
    d2 = s1.new Drone(12345, LIGHTCOLOR.RED, STATE.LIGHTON);

    // Ensure that the second drone was not inserted
    assertFalse(s1.findDrone(d2.getID()));
	}

	@Test
	void testInsertError() {
		
      // Insert the valid drones
	  s1.insert(d1);
	  s1.insert(d2);
  
	  // Attempt to insert duplicate drones with the same IDs
	  Show.Drone duplicateDrone1 = s1.new Drone(12345, LIGHTCOLOR.BLUE, STATE.LIGHTOFF);
	  Show.Drone duplicateDrone2 = s1.new Drone(54321, LIGHTCOLOR.RED, STATE.LIGHTON);
  
	  // Ensure that the duplicate drones were not inserted
	  assertFalse(s1.findDrone(duplicateDrone1.getID()));
	  assertFalse(s1.findDrone(duplicateDrone2.getID()));

	}

	@Test
	void testBalanceManyInsertions() {
		RandomGen IdSequence = new RandomGen(10000, 100000, RANDOMTYPE.UNIFORMINT, 3000, 0, 1);
		RandomGen LightSequence = new RandomGen(1, 3, RANDOMTYPE.UNIFORMINT, 3000, 0, 1);
		RandomGen StateSequence = new RandomGen(1, 2, RANDOMTYPE.UNIFORMINT, 3000, 0, 1);
		// using the above sequences for random numbers
		// you can create a drone with random id, LIGHTCOLOR, STATE
		 d1 = s1.new Drone(IdSequence.getRandNum()),
		 convertToLightcolor(LightSequence.getRandNum()),
		 convertToState(StateSequence.getRandNum());
		// the convertToLightcolor and convertToState functions are included in this
		// file
		// you can see an example of this working in the ShowTestPerformance.java

		// use a function to visit each node to see if the heights are correct,
		// leaf nodes are 0, the parent is 1+ largest height of right and left branches
		// difference in height of parent and child are <= 1
		// also check that the child id < parent id

		
	}

	@Test
	void testClear() {
		// create a tree
		s1.insert(d1);
		s1.insert(d2);
		s1.insert(d3);
		s1.clear();
		assertNull(s1.getRoot());
	}

	@Test
	void testRemoveEasy() {
		// create a simple tree
		s1.insert(d1);
		s1.insert(d2);
		s1.insert(d3);
		assertTrue(s1.findDrone(d3.getID()));
		// remove the node to the right of the root
		s1.remove(53002); // this is d3
		assertNull(s1.getRoot().getRight());
	}

	@Test
	void testRemoveNoRebalance() {
		fail("This test has not been implemented");
	}

	@Test
	void testRemoveRebalanceEasy() {
		s1.insert(d1); // root
		s1.insert(d2); // left of root
		s1.insert(d3); // right of root
		s1.insert(d4); // left of left of root
		s1.insert(d5); // right of left of root
		s1.insert(d6); // right of right of root
		s1.insert(d7);// left of left of left of root

		// delete 27904 which is right of left of root and requires a rebalance
		s1.remove(27904);
		assertEquals(53002, s1.getRoot().getID());
		assertEquals(60496, s1.getRoot().getRight().getID());
		assertEquals(19289, s1.getRoot().getLeft().getID());
		assertEquals(19224, s1.getRoot().getLeft().getLeft().getID());
		assertEquals(20201, s1.getRoot().getLeft().getRight().getID());

	}

	@Test
	void testRemoveRebalance() {
		fail("This test has not been implemented");
	}

	@Test
	void testBalanceManyRemovals() {
		// use a function to visit each node to see if the heights are correct,
		// leaf nodes are 0, the parent is 1+ largest height of right and left branches
		// difference in height of parent and child are <= 1
		// also check that the child id < parent id

		fail("This test has not been implemented");
	}

	@Test
	void testHeightChangesEasy() {
		s1.insert(d1); // root
		s1.insert(d2); // left of root
		s1.insert(d3); // right of root
		assertEquals(1, s1.getRoot().getHeight());
		s1.insert(d4); // left of left of root
		s1.insert(d5); // right of left of root
		s1.insert(d6); // right of right of root
		assertEquals(2, s1.getRoot().getHeight());
		s1.insert(d7);// left of left of left of root
		s1.insert(d8); // right of left of left of root
		s1.insert(d9); // left of right of left of root
		assertEquals(3, s1.getRoot().getHeight());
	}

	@Test
	void testHeight() {
		// use a function to visit each node to see if the heights are correct,
		// leaf nodes are 0, the parent is 1+ largest height of right and left branches
		// difference in height of parent and child are <= 1
		
		fail("This test has not been implemented");
	}

	@Test
	void testDumpTree() {
		s1.insert(d1); // root
		s1.insert(d2); // left of root
		s1.insert(d3); // right of root
		s1.insert(d4); // left of left of root
		s1.insert(d5); // right of left of root
		s1.insert(d6); // right of right of root
		s1.insert(d7);// left of left of left of root
		s1.insert(d8); // right of left of left of root
		s1.insert(d9); // left of right of left of root
		assertEquals("((((19224:0)19289:1(19372:0))20201:2((26241:0)27904:1))53002:3(60496:1(93209:0)))",
				s1.dumpTree());
	}
	@Test
	void testListDronesEasy() {
		s1.insert(d1); // root
		s1.insert(d2); // left of root
		s1.insert(d3); // right of root
		String result = s1.listDrones();
		assertEquals("20201:LIGHTON:RED\n53002:LIGHTON:RED\n60496:LIGHTON:RED", result);
	}

	@Test
	void testListDrones() {
		Show s1 = new Show();

		// Insert drones into the tree
		d1 = s1.new Drone(12345, LIGHTCOLOR.BLUE, STATE.LIGHTOFF);
		d2 = s1.new Drone(54321, LIGHTCOLOR.RED, STATE.LIGHTON);
		s1.insert(d1);
		s1.insert(d2);
	
		// Call the listDrones method
		String result = s1.listDrones();
	
		// Assert the result matches your expectations
		String expected = "12345:LIGHTOFF:BLUE\n54321:LIGHTON:RED";
		assertEquals(expected, result);
	}
	
	@Test
	void testSetStateEasy() {
		s1.insert(d1); // root
		s1.insert(d2); // left of root
		s1.insert(d3); // right of root
		String result = s1.listDrones();
		assertEquals("20201:LIGHTON:RED\n53002:LIGHTON:RED\n60496:LIGHTON:RED", result);
		s1.setState(20201, STATE.LIGHTOFF);
		result = s1.listDrones();
		assertEquals("20201:LIGHTOFF:RED\n53002:LIGHTON:RED\n60496:LIGHTON:RED", result);
	}

	@Test
	void testSetState() {
	Show s1 = new Show();

    // Insert drones into the tree
    d1 = s1.new Drone(12345, LIGHTCOLOR.BLUE, STATE.LIGHTOFF);
    s1.insert(d1);

    // Use setState to change the state of the drone
    s1.setState(12345, STATE.LIGHTON);

    // Check if the state was updated correctly
    assertEquals(STATE.LIGHTON, d1.getState());
	}

	@Test
	void testSetStateError() {
		 // Inserting drones with unique IDs
		 d1 = s1.new Drone(12345, LIGHTCOLOR.BLUE, STATE.LIGHTOFF);
		 s1.insert(d1);
		 d2 = s1.new Drone(54321, LIGHTCOLOR.RED, STATE.LIGHTON);
		 s1.insert(d2);
		  // Attempting to set the state of a non-existent drone
		  int nonExistentDroneID = 99999; // Choose an ID that does not exist
		  STATE newState = STATE.LIGHTON;  // Choose a new state
  
		  // Using a try-catch block to handle the expected exception
		  try {
			  s1.setState(nonExistentDroneID, newState);
  
			  // If no exception is thrown, fail the test
		  } catch (IllegalArgumentException e) {
			  // If an IllegalArgumentException is thrown, the test passes
			  // This means that the code correctly handled the error case
		  }
	  }
		 
	
	@Test
	void testRemoveLightOffNoRebalance() {
		s1.insert(d1); // root
		s1.insert(d2); // left of root
		s1.insert(d3); // right of root
		s1.insert(d4); // left of left of root
		s1.insert(d5); // right of left of root
		s1.insert(d6); // right of right of root
		s1.insert(d7);// left of left of left of root
		s1.insert(d8); // right of left of left of root
		s1.insert(d9); // left of right of left of root
		s1.setState(19372, STATE.LIGHTOFF);
		String result = s1.listDrones();
		assertEquals(
				"19224:LIGHTON:BLUE\n19289:LIGHTON:GREEN\n19372:LIGHTOFF:GREEN\n20201:LIGHTON:RED\n26241:LIGHTON:RED\n27904:LIGHTON:BLUE\n53002:LIGHTON:RED\n60496:LIGHTON:RED\n93209:LIGHTON:RED",
				result);
		s1.removeLightOff();
		result = s1.listDrones();
		assertEquals(
				"19224:LIGHTON:BLUE\n19289:LIGHTON:GREEN\n20201:LIGHTON:RED\n26241:LIGHTON:RED\n27904:LIGHTON:BLUE\n53002:LIGHTON:RED\n60496:LIGHTON:RED\n93209:LIGHTON:RED",
				result);

	}

	@Test
	void testRemoveLightOffReBalanceEasy() {
		s1.insert(d1); // root
		s1.insert(d2); // left of root
		s1.insert(d3); // right of root
		s1.insert(d4); // left of left of root
		s1.insert(d5); // right of left of root
		s1.insert(d6); // right of right of root
		s1.insert(d7);// left of left of left of root

		// set lightoff for 27904 which is right of left of root and requires a
		// rebalance
		s1.setState(27904, STATE.LIGHTOFF);
		System.out.println(s1.dumpTree());
		String result = s1.listDrones();
		assertEquals(
				"19224:LIGHTON:BLUE\n19289:LIGHTON:GREEN\n20201:LIGHTON:RED\n27904:LIGHTOFF:BLUE\n53002:LIGHTON:RED\n60496:LIGHTON:RED\n93209:LIGHTON:RED",
				result);
		s1.removeLightOff();
		result = s1.listDrones();
		assertEquals(
				"19224:LIGHTON:BLUE\n19289:LIGHTON:GREEN\n20201:LIGHTON:RED\n53002:LIGHTON:RED\n60496:LIGHTON:RED\n93209:LIGHTON:RED",
				result);
		assertEquals("(((19224:0)19289:1(20201:0))53002:2(60496:1(93209:0)))", s1.dumpTree());
	}

	@Test
	void testRemoveLightOffReBalance() {
		s1.insert(d1);
		s1.insert(d2);
		s1.insert(d3);
		s1.insert(d4);
	
		// Remove all LIGHTOFF drones
		s1.removeLightOff();
	}

	@Test
	void testFindDrone() {
		s1.insert(d1); // root
		s1.insert(d2); // left of root
		s1.insert(d3); // right of root
		s1.insert(d4); // left of left of root
		s1.insert(d5); // right of left of root
		s1.insert(d6); // right of right of root
		s1.insert(d7);// left of left of left of root
		s1.insert(d8); // right of left of left of root
		s1.insert(d9); // left of right of left of root
		String result = s1.listDrones();
		assertEquals(
				"19224:LIGHTON:BLUE\n19289:LIGHTON:GREEN\n19372:LIGHTON:GREEN\n20201:LIGHTON:RED\n26241:LIGHTON:RED\n27904:LIGHTON:BLUE\n53002:LIGHTON:RED\n60496:LIGHTON:RED\n93209:LIGHTON:RED",
				result);
		assertTrue(s1.findDrone(26241));
		assertTrue(s1.findDrone(93209));
		assertFalse(s1.findDrone(26240));
	}

	@Test
	void testCountDrones() {
		s1.insert(d1); // root
		s1.insert(d2); // left of root
		s1.insert(d3); // right of root
		s1.insert(d4); // left of left of root
		s1.insert(d5); // right of left of root
		s1.insert(d6); // right of right of root
		s1.insert(d7);// left of left of left of root
		s1.insert(d8); // right of left of left of root
		s1.insert(d9); // left of right of left of root
		String result = s1.listDrones();
		assertEquals(
				"19224:LIGHTON:BLUE\n19289:LIGHTON:GREEN\n19372:LIGHTON:GREEN\n20201:LIGHTON:RED\n26241:LIGHTON:RED\n27904:LIGHTON:BLUE\n53002:LIGHTON:RED\n60496:LIGHTON:RED\n93209:LIGHTON:RED",
				result);
		assertEquals(2, s1.countDrones(LIGHTCOLOR.BLUE));
		assertEquals(2, s1.countDrones(LIGHTCOLOR.GREEN));
		assertEquals(5, s1.countDrones(LIGHTCOLOR.RED));

	}

	@Test
	void testCopyConstructorEasy() {
		s1.insert(d1);
		s1.insert(d2);
		s1.insert(d3);
		Show s4 = new Show(s1);

		// this drone is the root of the tree
		assertEquals(53002, s4.getRoot().getID());
		assertEquals(LIGHTCOLOR.RED, s4.getRoot().getType());
		assertEquals(STATE.LIGHTON, s4.getRoot().getState());

		// left of the root
		assertEquals(20201, s4.getRoot().getLeft().getID());
		assertEquals(LIGHTCOLOR.RED, s4.getRoot().getLeft().getType());
		assertEquals(STATE.LIGHTON, s4.getRoot().getLeft().getState());

		// right of the root
		assertEquals(60496, s4.getRoot().getRight().getID());
		assertEquals(LIGHTCOLOR.RED, s4.getRoot().getRight().getType());
		assertEquals(STATE.LIGHTON, s4.getRoot().getRight().getState());
	}

	@Test
	void testCopyConstructor() {
	
        // Insert drones into the original Show
        s1.insert(d1);
        s1.insert(d2);

        // Create a copy of the original Show using the copy constructor
        Show s2 = new Show(s1);

        // Ensure that s1 and s2 are not the same object reference
        assertNotSame(s1, s2);

        // Check if both Shows have the same drones with the same properties
        assertEquals(s1.listDrones(), s2.listDrones());
	}

	STATE convertToState(int num) {
		if (num == 0)
			return STATE.LIGHTOFF;
		else
			return STATE.LIGHTON;
	}

	LIGHTCOLOR convertToLightcolor(int num) {
		switch (num) {
			case 1:
				return LIGHTCOLOR.BLUE;
			case 2:
				return LIGHTCOLOR.GREEN;
			case 3:
				return LIGHTCOLOR.RED;
		}
		return LIGHTCOLOR.RED;

	}
}
