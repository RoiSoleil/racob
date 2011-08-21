package org.racob.com;

import java.util.Date;

import org.racob.test.BaseTestCase;

/**
 * This class should test some of the converter capabilities
 * 
 */
public class VariantUtilitiesTest extends BaseTestCase {

	/**
	 * verify that dispatch can convert from object to variant and that the
	 * variant holds the right value
	 */
	public void testConverters() {
		Date testDate = new Date();
		Variant fromDate = VariantUtilities.objectToVariant(testDate);
		Date returnedDate = fromDate.getJavaDate();
		// System.out.println("test date is "+testDate);
		// System.out.println("VariantDate is "+fromDate.getJavaDate());
		assertTrue("Could not call obj2variant(Date) and get it to work",
				testDate.equals(returnedDate));
		Currency someMoney = new Currency(12349876L);
		Variant fromMoney = VariantUtilities.objectToVariant(someMoney);
		Currency someMoneyConverted = fromMoney.getCurrency();
		assertTrue("Could not call obj2variant(Long) and get it to work",
				someMoney.equals(someMoneyConverted));
		System.out.println("currency returned was: " + someMoneyConverted);

	}
}
