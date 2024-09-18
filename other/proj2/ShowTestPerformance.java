import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class ShowTestPerformance {
	Show s1,s2,s3;
	Show.Drone d0,d1,d2,d3,d4,d5,d6,d7,d8,d9;
	
	@BeforeEach
	void setUp() throws Exception {
		s2 = new Show();
		s1 = new Show();
        d0 = s1.new Drone();
	}

	@AfterEach
	void tearDown() throws Exception {
		s2 = s1 = null;
	}

	@Test
    void testInsertPerformance(){
        //run this test 10 times, if Acceptable > 6 times, passes
        int count = 0;
        for(int i=0; i<10; i++){
            if(testInsertPerformanceAcceptable()){
                count++;
            }
        }
        if(s1.getRoot()==null)
            assertTrue(false, "This is an empty tree");
        if(count >= 7) assertTrue(true, "Acceptable:");
        else assertTrue(false, "Unacceptable:");
    }

	boolean testInsertPerformanceAcceptable() {
        RandomGen IdSequence = new RandomGen(10000,100000,RANDOMTYPE.UNIFORMINT,3000,0,1);
		RandomGen LightSequence = new RandomGen(1,3,RANDOMTYPE.UNIFORMINT,3000,0,1);
		RandomGen StateSequence = new RandomGen(1,2,RANDOMTYPE.UNIFORMINT,3000,0,1);
		
        int show1Size = 1000;
        int show2Size = 2000;

        //find time for adding 1000 drones
        long startTime1000 = System.nanoTime();
        for(int i=0;i<show1Size; i++){
            d1 = s1.new Drone(IdSequence.getRandNum(), 
            convertToLightcolor(LightSequence.getRandNum()),
            convertToState(StateSequence.getRandNum()));

            s1.insert(d1);
        }
        long endTime1000 = System.nanoTime();
        long duration1000 = (endTime1000 - startTime1000);

        //find time for adding 2000 ships
        long startTime2000 = System.nanoTime();
        for(int i=0;i<show2Size; i++){
            d2 = s2.new Drone(IdSequence.getRandNum(), 
            convertToLightcolor(LightSequence.getRandNum()),
            convertToState(StateSequence.getRandNum()));

            s2.insert(d2);
        }
        long endTime2000 = System.nanoTime();
        long duration2000 = (endTime2000 - startTime2000);
        double compareTime = duration2000/(2.0 * duration1000);
        if(compareTime > .4 && compareTime < 1.1)
            return true;
        else return false;
	}

    STATE convertToState(int num)
    {
        if(num==0) return STATE.LIGHTOFF;
        else return STATE.LIGHTON;
    }

    LIGHTCOLOR convertToLightcolor(int num) {
        switch(num) {
            case 1: return LIGHTCOLOR.BLUE;
            case 2: return LIGHTCOLOR.GREEN;
            case 3: return LIGHTCOLOR.RED; 
        }
        return LIGHTCOLOR.RED;

    }

}
