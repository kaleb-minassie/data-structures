//import java.lang.Math;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.Arrays;


enum RANDOMTYPE {UNIFORMINT, UNIFORMREAL, NORMAL};
public class RandomGen {
    public RandomGen(int min, int max, RANDOMTYPE type, int sampleSize, int mean, int stdev) {
    	//creates a random set of integers or doubles starting from min up to but not including max
    	BigDecimal bd;
    	double result;
    	//10 is the fixed seed value
    	rand = new java.util.Random(10L);
    	
    	m_type = type;
    	m_min = min;
    	m_max = max;
    	m_sampleSize = sampleSize;
    	m_mean = mean;
    	m_stdev = stdev;
        if (type == RANDOMTYPE.NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
        	//nextGaussian creates a random number from a normal distribution with mean and standard deviation
        	// Using a fixed seed value generates always the same sequence
        	normalDistribution = new int[m_sampleSize];
        	for(int i=0;i<m_sampleSize;i++ ) {
        		result = rand.nextGaussian(m_mean, m_stdev);
        		if(result < m_min || result >= m_max) { i--; continue;}
        		normalDistribution[i] = (int)result;
        	}
        }
        else if (type == RANDOMTYPE.UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
        	
        	uniformIntDistribution = rand.ints(m_sampleSize, m_min, m_max).toArray();
        }
        else{ //the case of UNIFORMREAL to generate real numbers
        	double[] tempDistribution;
        	uniformRealDistribution = new double[m_sampleSize];
        	//create an array of doubles between m_min and m_max
        	tempDistribution = rand.doubles(m_sampleSize, m_min, m_max).toArray();
        	//convert them to 2 decimal values
        	for(int i=0;i<m_sampleSize;i++) {
        		result = tempDistribution[i];
        		bd = new BigDecimal(Double.toString(result));
        		bd = bd.setScale(2,RoundingMode.HALF_UP);
        		uniformRealDistribution [i] = bd.doubleValue();
        	}
        }
    }
    
    public RandomGen(int min, int max){
    	//generates a uniform integer distribution with a sample size of 100
    	m_min = min;
    	m_max = max;
    	m_sampleSize = 100;
    	m_type = RANDOMTYPE.UNIFORMINT;
    	//10 is the fixed seed value
    	rand = new java.util.Random(10L);

    	//the case of UNIFORMINT to generate integer numbers
    	// Using a fixed seed value generates always the same sequence
    	// of pseudorandom numbers, e.g. reproducing scientific experiments
    	// here it helps us with testing since the same sequence repeats
    	
    	//store 100 random numbers between m_min and m_max
    	uniformIntDistribution = rand.ints(m_sampleSize, m_min, m_max).toArray();
    }

    public void printDistribution(String type) {
    	switch(type) {
    	case "uniformInt":
    		for(int i=0;i<m_sampleSize;i++)
        		System.out.print(uniformIntDistribution[i]+" ");
    		System.out.print("\n");
    		break;
    	case "uniformReal":
    		for(int i=0;i<m_sampleSize;i++)
        		System.out.print(uniformRealDistribution[i]+" ");
    		System.out.print("\n");
    		break;
    	case "normal":
    		for(int i=0;i<m_sampleSize;i++)
        		System.out.print(normalDistribution[i]+" ");
    		System.out.print("\n");
    		break;
    	}
    }
    
    public void printDistributionOrder(String type) {
    	switch(type) {
    	case "uniformInt":
    		Arrays.sort(uniformIntDistribution);
    		for(int i=0;i<m_sampleSize;i++)
        		System.out.print(uniformIntDistribution[i]+" ");
    		System.out.print("\n");
    		break;
    	case "uniformReal":
    		Arrays.sort(uniformRealDistribution);
    		for(int i=0;i<m_sampleSize;i++)
        		System.out.print(uniformRealDistribution[i]+" ");
    		System.out.print("\n");
    		break;
    	case "normal":
    		Arrays.sort(normalDistribution);
    		for(int i=0;i<m_sampleSize;i++)
        		System.out.print(normalDistribution[i]+" ");
    		System.out.print("\n");
    		break;
    	}
    }
    
    public void printDistributionVisual(String type) {
    	int size,j;
    	switch(type) {
    	case "uniformInt":
    		System.out.println("Uniform Integer Distribution for "+m_sampleSize+" integers in range "+m_min+"(inclusively) to "+m_max+"(exclusively)");
    		//size is the number of possible integers
    		size = m_max - m_min;
    		int[] result = new int[size];
    		//initialize result
    		for(int i=0;i<size;i++)
    			result[i]= 0;
    		//determine how many times a number appears
    		for(int i=0;i<m_sampleSize;i++)
        		result[uniformIntDistribution[i]-m_min]++;
    		for(int i=0;i<size;i++) {
    			System.out.print(i+m_min+":");
    			for(j=0; j<result[i];j++)
    				System.out.print("*");
    			System.out.print("\n");
    		}
    		break;
    	case "uniformReal":
    		System.out.println("Uniform Real Distribution for "+m_sampleSize+" real numbers in range "+m_min+"(inclusively) to "+m_max+"(exclusively)");
    		//size is the number of possible integers
    		size = m_max - m_min;
    		double[] resultD = new double[size];
    		//initialize result
    		for(int i=0;i<size;i++)
    			resultD[i]= 0;
    		//determine how many times a number appears
    		for(int i=0;i<m_sampleSize;i++)
        		resultD[(int)(uniformRealDistribution[i]-m_min)]++;
    		for(int i=0;i<size;i++) {
    			System.out.print(i+m_min+".00-"+(i+m_min)+".99:");
    			for(j=0; j<resultD[i];j++)
    				System.out.print("*");
    			System.out.print("\n");
    		}
    		break;
    	case "normal":
    		System.out.println("Normal Distribution for "+m_sampleSize+" numbers in range "+m_min+"(inclusively) to "+m_max+"(exclusively)"
    				+ " mean of "+m_mean+" standard deviation of "+m_stdev);
    		//size is the number of possible integers
    		//size = m_max-m_min;
    		size = m_max - m_min;
    		int[] resultN = new int[size];
    		for(int i=0;i<m_sampleSize;i++) {
    			resultN[normalDistribution[i]-m_min]++;
    		}
    		for(int i=0;i<size;i++) {
    			System.out.print((i+m_min)+":");
    			for(j=0; j<resultN[i];j++)
    				System.out.print("*");
    			System.out.print("\n");
    		}
    		
    		break;
    	}
    }
    
    public int getRandNum(){
    	// this function returns integer numbers
    	// the object must have been initialized to generate integers
    	if (m_type == RANDOMTYPE.NORMAL){
    		//the case of NORMAL to generate a random integer number with normal distribution
    		//nextGaussian creates a random number from a normal distribution with mean and standard deviation
    		// Using a fixed seed value generates always the same sequence
    		double result = -1;
    		while(result < m_min || result >= m_max) {
    			result = rand.nextGaussian(m_mean, m_stdev);
    		}
    		return (int)result;
    	}
    	else if(m_type == RANDOMTYPE.UNIFORMINT) {
    		//the case of UNIFORMINT to generate integer numbers
    		// Using a fixed seed value generates always the same sequence
    		// of pseudorandom numbers, e.g. reproducing scientific experiments
    		// here it helps us with testing since the same sequence repeats

    		return rand.nextInt(m_min, m_max);
    	}
    	else return -1;

    }

    public double getRealRandNum(){
    	// this function returns real numbers
    	// the object must have been initialized to generate real numbers
    	double result = -1;
    	while(result < m_min || result >= m_max)
    		result = rand.nextDouble(m_min, m_max);
    	//convert them to 2 decimal values

    	BigDecimal bd = new BigDecimal(Double.toString(result));
    	bd = bd.setScale(2,RoundingMode.HALF_UP);
    	return bd.doubleValue();
    }

    
    private int m_min;
    private int m_max;
    private int m_sampleSize;
    private int m_mean;
    private int m_stdev;
    private RANDOMTYPE m_type;
    private int[] uniformIntDistribution;
    private double[] uniformRealDistribution;
    private int[] normalDistribution;
    java.util.Random rand;
    
}
