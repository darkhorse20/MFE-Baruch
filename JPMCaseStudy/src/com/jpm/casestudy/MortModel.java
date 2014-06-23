package com.jpm.casestudy;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.StringTokenizer;


/**
 * @author Megha
 *
 */
public class MortModel {

	private List<Deal> deals = new ArrayList<Deal>();
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println(System.getProperty("user.dir"));
		String dealsFile = args[0];
		String econVarsFile = args[1];
		String startDate = args[2];
		String endDate = args[3];
		
		MortModel mortModel = new MortModel();
		mortModel.loadDeals(dealsFile);
		EconomicVariables eVars = new EconomicVariables(econVarsFile);
		for(Deal deal: mortModel.getDeals()) {
			//AmortizationSchedule sch = new AmortizationSchedule(834837.54, 0.05, 49, 60, 24, 12, "Config.csv", "09/1/2013", "10/01/2015");
			//AmortizationSchedule sch = new AmortizationSchedule(deal, eVars, Utils.formatter.format(deal.getOrigDate()), "10/01/2015");
			AmortizationSchedule sch = new AmortizationSchedule(deal, eVars, "10/01/2013", "10/01/2015");
			sch.calcSchedule(false);
			sch.saveSchedule();

		}	
	}
	
/*	Date date, String state, Double amt, Double ltv,
	 Double fico,
	 Integer term,
	 Integer fixTerm,
	 Double fixRate,
	 String indx,
	 Double bal,
	 Integer age,
	 Double offset*/
	public  void loadDeals(String fileConf) {
			try {
				
				FileReader r = new FileReader(fileConf);
				BufferedReader b = new BufferedReader(r);
				String line = b.readLine();
				while(line !=null) {
					StringTokenizer t = new StringTokenizer(line,",");
					if(t.countTokens() > 0) {
					     deals.add(
					            new Deal( 
					            		Utils.formatter.parse(t.nextToken()),
					            		t.nextToken(),
					                    Double.parseDouble(t.nextToken()), 
					                    Double.parseDouble(t.nextToken()),
					                    Double.parseDouble(t.nextToken()), 
					                    Integer.parseInt(t.nextToken()),
					                    Integer.parseInt(t.nextToken()),
					                    Double.parseDouble(t.nextToken()),
					                    t.nextToken(),
					                    Double.parseDouble(t.nextToken()), 
					                    Integer.parseInt(t.nextToken()),
					                    Double.parseDouble(t.nextToken())
					                      )
					    );
					}
					line = b.readLine();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}

	

	public List<Deal> getDeals() {
		return this.deals;
	}

}
