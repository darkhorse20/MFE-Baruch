package com.jpm.casestudy;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
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
	private Double r12;
	private EconomicVariables eVars;
	private ArrayList<Record> schedule = new ArrayList<AmortizationSchedule.Record>(60);
	
	private Calendar cal = Calendar.getInstance();
	
	
	public AmortizationSchedule(Double amt, Double rate, int N, int term, int fixedTerm, int age, String confFile, String startDate) {
		eVars = new EconomicVariables(confFile);
		this.A = amt;
		this.r = rate;
		this.N = N;
		this.term = term;
		this.fixedTerm = fixedTerm;
		this.age = age;
		
		try {
			this.startDate = Utils.formatter.parse(startDate);
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
	
	public void calcSchedule() {
		int nrYears = term - age +1;
		double remainingDebt = A;
		double z = (1+r12);
		if(age < fixedTerm) {
			Double mthly = getMonthlyPayment();
			Double principal = mthly - remainingDebt*r/12;
			int j=0;
			for (int i=0; i < nrYears; i++) {
				if(i<(fixedTerm - age)) { //Do fixed rate schedule
					
					schedule.add(new Record(addMonths(i), age+i, r, remainingDebt, principal, mthly - principal));
					
					remainingDebt = remainingDebt*z - mthly;
					principal = mthly - remainingDebt*r12; 
					
				} else {//We are in ARM
					
					schedule.add(new Record(addMonths(i), age+i, r, remainingDebt, principal, mthly - principal));
					
					//double flRate = (0.01*(eVars.getLibor().get(j))+0.02)/12;
					double flRate = (0.01*0.414476+0.02)/12;

					mthly = getMonthlyPayment(remainingDebt, flRate, 37);
					remainingDebt = remainingDebt*(1+ flRate) - mthly;
					principal = mthly - remainingDebt*flRate;
					j++;
				}
			}
			
		} else {
			//We're passed the fixed term. It is assumed a starting balance and index rate is available.
		}
		
	}
	
	public void calcSchedule(Double amt, Double rate, Double N) {
		
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
		Utils.write(schedule, "C:\\ac-fe\\JPMCaseStudy\\schedule.xls");
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
