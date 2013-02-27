/**
 * 
 */
package com.kingstargroup.form;

/**<br>
 * Copyright (C), 2000-2005, Kingstar Co., Ltd. <br>
 * File name: DupliSerialResult.java<br>
 * Description: <br>
 * Modify History����Change Log��:  <br>
 * �������ͣ��������޸ĵȣ�   ��������     ������    �������ݼ���<br>
 * ����  				 2005-11-2  ������    <br>
 * <p>
 *
 * @author      ������
 * @version     1.0
 * @since       1.0
 */
public class DupliSerialResult extends AbstractResult {


    /** The value of the SERIAL_NO component of this composite id. */
    private java.lang.Integer serialNo;
    
    /** The value of the DEVICE_ID component of this composite id. */
    private java.lang.String deviceId;

    /** The value of the CARD_NO component of this composite id. */
    private java.lang.Integer cardNo;
	
	/**
	 * @param no
	 * @param id
	 * @param no2
	 */
	public DupliSerialResult(Integer serialNo, String deviceId, Integer cardNo) {
		this.cardNo = cardNo;
		this.deviceId = deviceId;
		this.serialNo = serialNo;
	}

	public java.lang.Integer getCardNo() {
		return cardNo;
	}

	public void setCardNo(java.lang.Integer cardNo) {
		this.cardNo = cardNo;
	}

	public java.lang.String getDeviceId() {
		return deviceId;
	}

	public void setDeviceId(java.lang.String deviceId) {
		this.deviceId = deviceId;
	}

	public java.lang.Integer getSerialNo() {
		return serialNo;
	}

	public void setSerialNo(java.lang.Integer serialNo) {
		this.serialNo = serialNo;
	}


}