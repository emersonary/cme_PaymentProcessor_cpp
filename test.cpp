#include "pch.h"
#include "PaymentProcessor.cpp"

TEST(TestCaseName, TestName) {

	PaymentProcessor paymentProcessor;

	const int idAccountEmerson  = paymentProcessor.createAccount("EMERSON", 100);
	const int idAccountEmerson2 = paymentProcessor.createAccount("EMERSON2", 200);
	
    EXPECT_EQ(paymentProcessor.getAccountBalance(idAccountEmerson), 100);
    EXPECT_EQ(paymentProcessor.getAccountBalance(idAccountEmerson2), 200);

	paymentProcessor.processTransaction(20, idAccountEmerson, idAccountEmerson2);

	EXPECT_EQ(paymentProcessor.getAccountBalance(idAccountEmerson), 80);
	EXPECT_EQ(paymentProcessor.getAccountBalance(idAccountEmerson2), 220);

	EXPECT_EQ(paymentProcessor.TransactionCount(), 1);


  //EXPECT_TRUE(true);

}