/**
 * Copyright (c) 2000-2005 Liferay, LLC. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

package com.liferay.portlet.mail.model;

import com.liferay.util.Validator;

import java.util.Date;

/**
 * <a href="MailReceipt.java.html"><b><i>View Source</i></b></a>
 *
 * @author  Brian Wing Shun Chan
 * @version $Revision: 1.4 $
 *
 */
public class MailReceipt extends MailReceiptModel {

	public MailReceipt() {
		super();
	}

	public MailReceipt(String receiptId) {
		super(receiptId);
	}

	public MailReceipt(String receiptId, String companyId, String userId,
					   Date createDate, Date modifiedDate, String recipientName,
					   String recipientAddress, String subject, Date sentDate,
					   int readCount, Date firstReadDate, Date lastReadDate) {

		super(receiptId, companyId, userId, createDate, modifiedDate,
			  recipientName, recipientAddress, subject, sentDate, readCount,
			  firstReadDate, lastReadDate);
	}

	public String getRecipient() {
		if (Validator.isNull(getRecipientName())) {
			return getRecipientAddress();
		}
		else {
			return getRecipientName() + "<" + getRecipientAddress() + ">";
		}
	}

}