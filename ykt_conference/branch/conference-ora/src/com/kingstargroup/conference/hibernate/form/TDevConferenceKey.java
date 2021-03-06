/*
 * WARNING: DO NOT EDIT THIS FILE. This is a generated file that is synchronized
 * by MyEclipse Hibernate tool integration.
 *
 * Created Thu May 25 09:58:48 CST 2006 by MyEclipse Hibernate Tool.
 */
package com.kingstargroup.conference.hibernate.form;

import java.io.Serializable;

/**
 * A class representing a composite primary key id for the T_DEV_CONFERENCE
 * table.  This object should only be instantiated for use with instances 
 * of the TDevConference class.
 * WARNING: DO NOT EDIT THIS FILE. This is a generated file that is synchronized 
 * by MyEclipse Hibernate tool integration.
 */
public class TDevConferenceKey
    implements Serializable
{
    /** The cached hash code value for this instance.  Settting to 0 triggers re-calculation. */
    private volatile int hashValue = 0;

    /** The value of the CON_ID component of this composite id. */
    private java.lang.Integer conId;

    /** The value of the DEVICE_ID component of this composite id. */
    private java.lang.Integer deviceId;

    /**
     * Simple constructor of TDevConferenceKey instances.
     */
    public TDevConferenceKey()
    {
    }

    /**
     * Returns the value of the conId property.
     * @return java.lang.Integer
     */
    public java.lang.Integer getConId()
    {
        return conId;
    }

    /**
     * Sets the value of the conId property.
     * @param conId
     */
    public void setConId(java.lang.Integer conId)
    {
        hashValue = 0;
        this.conId = conId;
    }

    /**
     * Returns the value of the deviceId property.
     * @return java.lang.Integer
     */
    public java.lang.Integer getDeviceId()
    {
        return deviceId;
    }

    /**
     * Sets the value of the deviceId property.
     * @param deviceId
     */
    public void setDeviceId(java.lang.Integer deviceId)
    {
        hashValue = 0;
        this.deviceId = deviceId;
    }

    /**
     * Implementation of the equals comparison on the basis of equality of the id components.
     * @param rhs
     * @return boolean
     */
    public boolean equals(Object rhs)
    {
        if (rhs == null)
            return false;
        if (! (rhs instanceof TDevConferenceKey))
            return false;
        TDevConferenceKey that = (TDevConferenceKey) rhs;
        if (this.getConId() == null || that.getConId() == null)
        {
            return false;
        }
        if (! this.getConId().equals(that.getConId()))
        {
            return false;
        }
        if (this.getDeviceId() == null || that.getDeviceId() == null)
        {
            return false;
        }
        if (! this.getDeviceId().equals(that.getDeviceId()))
        {
            return false;
        }
        return true;
    }

    /**
     * Implementation of the hashCode method conforming to the Bloch pattern with
     * the exception of array properties (these are very unlikely primary key types).
     * @return int
     */
    public int hashCode()
    {
        if (this.hashValue == 0)
        {
            int result = 17;
            int conIdValue = this.getConId() == null ? 0 : this.getConId().hashCode();
            result = result * 37 + conIdValue;
            int deviceIdValue = this.getDeviceId() == null ? 0 : this.getDeviceId().hashCode();
            result = result * 37 + deviceIdValue;
            this.hashValue = result;
        }
        return this.hashValue;
    }
}
