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

package com.liferay.portlet.journal.service.spring;

/**
 * <a href="JournalTemplateService.java.html"><b><i>View Source</i></b></a>
 *
 * @author  Brian Wing Shun Chan
 * @version $Revision: 1.183 $
 *
 */
public interface JournalTemplateService {
	public com.liferay.portlet.journal.model.JournalTemplate addTemplate(
		java.lang.String templateId, boolean autoTemplateId,
		java.lang.String portletId, java.lang.String groupId,
		java.lang.String structureId, java.lang.String name,
		java.lang.String description, java.lang.String xsl, boolean formatXsl,
		boolean smallImage, java.lang.String smallImageURL,
		java.io.File smallFile)
		throws com.liferay.portal.PortalException, 
			com.liferay.portal.SystemException, java.rmi.RemoteException;

	public void deleteTemplate(java.lang.String templateId)
		throws com.liferay.portal.PortalException, 
			com.liferay.portal.SystemException, java.rmi.RemoteException;

	public com.liferay.portlet.journal.model.JournalTemplate getTemplate(
		java.lang.String templateId)
		throws com.liferay.portal.PortalException, 
			com.liferay.portal.SystemException, java.rmi.RemoteException;

	public java.util.List getTemplates(java.lang.String portletId,
		java.lang.String groupId, java.lang.String companyId, int begin, int end)
		throws com.liferay.portal.SystemException, java.rmi.RemoteException;

	public java.util.List getTemplates(java.lang.String structureId)
		throws com.liferay.portal.SystemException, java.rmi.RemoteException;

	public java.util.List getTemplates(java.lang.String structureId, int begin,
		int end)
		throws com.liferay.portal.SystemException, java.rmi.RemoteException;

	public int getTemplatesSize(java.lang.String portletId,
		java.lang.String groupId, java.lang.String companyId)
		throws com.liferay.portal.SystemException, java.rmi.RemoteException;

	public int getTemplatesSize(java.lang.String structureId)
		throws com.liferay.portal.SystemException, java.rmi.RemoteException;

	public com.liferay.portlet.journal.model.JournalTemplate updateTemplate(
		java.lang.String templateId, java.lang.String name,
		java.lang.String description, java.lang.String xsl, boolean formatXsl,
		boolean smallImage, java.lang.String smallImageURL,
		java.io.File smallFile)
		throws com.liferay.portal.PortalException, 
			com.liferay.portal.SystemException, java.rmi.RemoteException;
}