package com.jpm.casestudy;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;

public class AmortizationSchedule {
	private Double A;
	private Double r;
	private Integer N;
	private Integer term;
	private Integer fixedTerm;
	private Integer age;
	private Date startDate;
	private Date endDate;
	private Double r12;
	private EconomicVariables eVars;
	private ArrayList<Record> schedule = new ArrayList<AmortizationSchedule.Record>(60);
	
	private Calendar cal = Calendar.getInstance();
	private Deal deal;
	
	public AmortizationSchedule(Deal deal, EconomicVariables  econVars, String stDate, String endDate) {
		eVars = econVars;
		this.A = deal.getBalance();
		this.r = deal.getFixedRate();
		this.N = (deal.getTerm() - deal.getAge() +1);
		this.term = deal.getTerm();
		this.fixedTerm = deal.getFixedTerm();
		this.age = deal.getAge();
		
		try {
			this.startDate = Utils.formatter.parse(stDate);
			this.endDate = Utils.formatter.parse(endDate);
			
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Bad DAte");
		}
		
		cal.setTime(this.startDate);
		r12 = r/12; //Set mthly rate
		this.deal = deal;
	}
	
	public AmortizationSchedule(Double amt, Double rate, int N, int term, int fixedTerm, int age, String confFile, String startDate, String endDate) {
		eVars = new EconomicVariables(confFile);
		this.A = amt;
		this.r = rate;
		this.N = N;
		this.term = term;
		this.fixedTerm = fixedTerm;
		this.age = age;
		
		try {
			this.startDate = Utils.formatter.parse(startDate);
			this.endDate = Utils.formatter.parse(endDate);
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Bad DAte");
		}
		
		cal.setTime(this.startDate);
		r12 = r/12; //Set mthly rate
	}
	
	public AmortizationSchedule(String confFile) {
		eVars = new EconomicVariables(confFile);
		
	}
	
	private Date addMonths(int nr) { 
		cal.add(Calendar.MONTH, nr);
		return cal.getTime();
	}
	
	public void calFullSchedule() {
		this.age = 0;
		calcSchedule(false);
	}
	
	public void calcSchedule(boolean probablisitic) {
		int nrYears = term - age +1;
		double remainingDebt = A;
		double z = (1+r12);
		double probPrePay=0, probDefault=0;
		
	//	if(age < fixedTerm) {
			Double mthly = getMonthlyPayment();
			Double principal = 0.0d; //mthly - remainingDebt*r/12;
			Date currDate = cal.getTime();
		
			for (int i=0; i < nrYears; i++) {
			    if(currDate.after(endDate) || currDate.equals(endDate)) {
			    	break;
			    }
			    
				if(i<=(fixedTerm - age)) { //Do fixed rate schedule
				    
                    principal = mthly - remainingDebt*r12;
                    
					schedule.add(new Record(currDate, age+i, r, remainingDebt, principal, mthly - principal));
					
					if(probablisitic) {
						probPrePay = getProbOfPrePay(r12, eVars.getMtgRate(currDate));
						probDefault = getProbOfDefault(i, currDate, remainingDebt);
						remainingDebt = getProbOfPrePay(r12, eVars.getMtgRate(currDate)) + (1 - probPrePay - probDefault)*remainingDebt;
							
					} else
						remainingDebt = remainingDebt*z - mthly;
					
				} else {//We are in ARM
					Double flRate = (0.01*eVars.getLibor(currDate)+deal.getOffset())/12;
                    //double flRate = (0.01*0.414476 + 0.02)/12;
                    
				    mthly = getMonthlyPayment(remainingDebt, flRate, term - age -i +1);
				    principal = mthly - remainingDebt*flRate;
				    
					schedule.add(new Record(currDate, age+i, flRate*12, remainingDebt, principal, mthly - principal));

					if(probablisitic) {
						probPrePay = getProbOfPrePay(flRate, eVars.getMtgRate(currDate));
						probDefault = getProbOfDefault(i, currDate, remainingDebt);
						remainingDebt = getProbOfPrePay(flRate, eVars.getMtgRate(currDate)) + (1 - probPrePay - probDefault)*remainingDebt;
							
					} else
						remainingDebt = remainingDebt*(1+ flRate) - mthly;					
				
				}
				currDate = addMonths(1);
			}
			
//		} else {
//			//We're passed the fixed term. It is assumed a starting balance and index rate is available.
//		}
		
	}
	
	/** LTV[t] := updated loan-to-value ratio at point t := BAL[t]/( (OBAL/OLTV) * (HPI[state,
	                                                                                t]/HPI[state, 0]))
	 * @param time
	 * @return
	 */
	private Double getLTV(Integer time, Date dt, Double balance) {
		return balance/( (deal.getOrigAmt()/deal.getoLtv()) * (eVars.getHPI(dt, deal.getState())/eVars.getHPI(deal.getOrigDate(), deal.getState())));
	}
	
	/**
	 * PD[t | survived up to t-1, doesn’t prepay at t] := 
	 * 1/(1+exp(4.4+0.01*OFICO-4*LTV[t]+0.2*min(max(AGE-36, 0),24)+0.1*min(max(AGE[t]-60, 0), 60)-0.05*min(max(AGE[t]-120, 0),140)))
	 * @return
	 */
	private Double getProbOfDefault(Integer time, Date dt, Double balance) {
		return 1/( 1 + Math.exp(4.4 + 0.01*deal.getoFico() 
				- 4*getLTV(time, dt, balance) 
				+ 0.2*Math.min(Math.max(deal.getAge()-36,0), 24) 
				+ 0.1*Math.min(Math.max(time-60, 0), 60)
				- 0.05*Math.min(Math.max(time-120,0), 140)
		));
	}
	
	/**
	 * P_Prepay[t | survived up to t-1] := 1/(1 + exp(3.4761-101.09*(RATE[t]-MTG[t]))))
	 * @return
	 */
	private Double getProbOfPrePay(Double rate, Double mtgRate) {
		return 1/(1 + Math.exp(3.4761 - 101.09*(rate - mtgRate)));
	}
	
	public void calcExpBalance() {
		
	}
	
	public Double getMonthlyPayment(Double amt, Double rate, Integer N) {
		Double exp = Math.pow(1+rate, N);
		return amt*(exp*rate)/(exp -1);
	}
	
	public Double getMonthlyPayment() {
		//Double r12 = r/12;
		Double exp = Math.pow(1+r12, N);
		return A*(exp*r12)/(exp -1);
	}
	
	public void saveSchedule() {
		Utils.write(schedule, "C:\\ac-fe\\JPMCaseStudy\\schedules" + deal.getState() + ".xls");
	}

	public class Record {
		private Date date;
		private Integer age;
		private Double rate;
		private Double balance;
		private Double principal;
		private Double interest;
		
		public Record(Date date, Integer age, Double rate, Double balance, Double principal, Double interest) {
			this.date = date;
			this.age = age;
			this.rate = rate;
			this.balance = balance;
			this.principal = principal;
			this.interest = interest;
			
		}

		public Date getDate() {
			return date;
		}

		public void setDate(Date date) {
			this.date = date;
		}

		public Integer getAge() {
			return age;
		}

		public void setAge(Integer age) {
			this.age = age;
		}

		public Double getRate() {
			return rate;
		}

		public void setRate(Double rate) {
			this.rate = rate;
		}

		public Double getBalance() {
			return balance;
		}

		public void setBalance(Double balance) {
			this.balance = balance;
		}

		public Double getPrincipal() {
			return principal;
		}

		public void setPrincipal(Double principal) {
			this.principal = principal;
		}

		public Double getInterest() {
			return interest;
		}

		public void setInterest(Double interest) {
			this.interest = interest;
		}
		
		
 	}
}
