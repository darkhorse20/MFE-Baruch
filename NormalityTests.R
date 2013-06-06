library(moments)
library(randtoolbox)
N <- 30000
setwd("C:/aya/Documents/MFE/Baruch/Volatility Filtering and Estimation/FinalProject/Data")

observation <- read.csv("ObsFileSqrt.csv", header=FALSE)
estimates <- read.csv("EstFileSqrt.csv", header=FALSE)
means <- read.csv("MeanFileSqrt.csv", header=FALSE)
vars <- read.csv("VarianceFileSqrt.csv", header=FALSE)

std_residuals <- ((observation[,1] - estimates[,1]) - means[,1])/sqrt(vars[,1]);

hist_results <- hist(std_residuals, plot = TRUE, main="Sqrt p = 0.5")

(pnorm(1,0,1) - pnorm(0,0,1)) * 30000
intervals <- hist_results$breaks 
  
#c(-10, -9, -8, -7, -6, -5, -4, -3, -2, 
#-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12)

n <- length(intervals)
norm_bins <- (pnorm(intervals[-1],0,1) - pnorm(intervals[-n],0,1))*N
              
chisq_results <- chisq.test(hist_results$counts, norm_bins)

cat("Chi-Squared Stat: ", chisq_results$statistic)
cat("P-Value: ", chisq_results$p.value)


g_range <- range(0, hist_results$counts, norm_bins)
plot(hist_results$counts, ylab="Frequency", ylim=g_range, col = "blue", main="Normal vs EKF Estimates")
par(new = T)
plot(norm_bins,  ylab="Frequency", ylim=g_range, col = "orange")
legend(1, g_range[2],c("EKF","Normal"), cex=0.8, col=c("blue","orange"),
       pch=1:1)


qqnorm(std_residuals)

########## TEst for the GARCH Model #########
observation <- read.csv("ObsFileGARCH.csv", header=FALSE)
estimates <- read.csv("EstFileGARCH.csv", header=FALSE)
means <- read.csv("MeanFileGARCH.csv", header=FALSE)
vars <- read.csv("VarianceFileGARCH.csv", header=FALSE)

std_residuals <- ((observation[,1] - estimates[,1]) - means[,1])/sqrt(vars[,1]);

hist_results <- hist(std_residuals, plot = TRUE, main="GARCH p = 1.0")

(pnorm(1,0,1) - pnorm(0,0,1)) * 30000
intervals <- hist_results$breaks
#intervals <- c(-10, -9, -8, -7, -6, -5, -4, -3, -2, 
#               -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12)

n <- length(intervals)
norm_bins <- (pnorm(intervals[-1],0,1) - pnorm(intervals[-n],0,1))*N

chisq_results <- chisq.test(hist_results$counts, norm_bins)

cat("Chi-Squared Stat: ", chisq_results$statistic)
cat("P-Value: ", chisq_results$p.value)


g_range <- range(0, hist_results$counts, norm_bins)
plot(hist_results$counts, ylab="Frequency", ylim=g_range, col = "blue", main="Normal vs EKF Estimates")
par(new = T)
plot(norm_bins,  ylab="Frequency", ylim=g_range, col = "orange")
legend(1, g_range[2],c("EKF","Normal"), cex=0.8, col=c("blue","orange"),
       pch=1:1)


qqnorm(std_residuals)



######## Tests for 3-2 Model ###############3
observation <- read.csv("ObsFile32.csv", header=FALSE)
estimates <- read.csv("EstFile32.csv", header=FALSE)
means <- read.csv("MeanFile32.csv", header=FALSE)
vars <- read.csv("VarianceFile32.csv", header=FALSE)

std_residuals <- ((observation[,1] - estimates[,1]) - means[,1])/sqrt(vars[,1]);

hist_results <- hist(std_residuals, plot = TRUE, main="3-2 Model p = 1.5")

(pnorm(1,0,1) - pnorm(0,0,1)) * 30000
intervals <- c(-10, -9, -8, -7, -6, -5, -4, -3, -2, 
               -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12)

n <- length(intervals)
norm_bins <- (pnorm(intervals[-1],0,1) - pnorm(intervals[-n],0,1))*N

chisq_results <- chisq.test(hist_results$counts, norm_bins)

cat("Chi-Squared Stat: ", chisq_results$statistic)
cat("P-Value: ", chisq_results$p.value)

g_range <- range(0, hist_results$counts, norm_bins)
plot(hist_results$counts, ylab="Frequency", ylim=g_range, col = "blue", main="Normal vs EKF Estimates")
par(new = T)
plot(norm_bins,  ylab="Frequency", ylim=g_range, col = "orange")
legend(1, g_range[2],c("EKF","Normal"), cex=0.8, col=c("blue","orange"),
       pch=1:1)


qqnorm(std_residuals)