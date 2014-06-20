package com.jpm.casestudy;


public class MortModel {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println(System.getProperty("user.dir")); 
		AmortizationSchedule sch = new AmortizationSchedule(834837.54, 0.05, 49, 60, 24, 12, "Config.csv", "10/1/2013");
		sch.calcSchedule();
		sch.saveSchedule();
	}

}
