#pragma once

#include <iostream>

typedef struct _awe_jsarray awe_jsarray;
typedef enum _awe_cursor_type awe_cursor_type;
class UIView;

class UIViewListener
{
public:

	/**
	 * @fn	virtual void UIViewListener::OnBeginNavigation(UIView* uiview, const std::string& url,
	 * 		const std::string& frameName) =0;
	 *
	 * @brief	Executes the begin navigation action.
	 *
	 * @param [in,out]	uiview	The uiview.
	 * @param	url			  	URL of the document.
	 * @param	frameName	  	Name of the frame.
	 */
	virtual void OnBeginNavigation(UIView* uiview, const std::string& url, const std::string& frameName) =0;

	/**
	 * @fn	virtual void UIViewListener::OnBeginLoading(UIView* uiview, const std::string& url,
	 * 		const std::string& frameName, int status_code, const std::string& mime_type) =0;
	 *
	 * @brief	Executes the begin loading action.
	 *
	 * @param [in,out]	uiview	The uiview.
	 * @param	url			  	URL of the document.
	 * @param	frameName	  	Name of the frame.
	 * @param	status_code   	The status code.
	 * @param	mime_type	  	Type of the mime.
	 */
	virtual void OnBeginLoading(UIView* uiview, const std::string& url, const std::string& frameName, int status_code, const std::string& mime_type) =0;

	/**
	 * @fn	virtual void UIViewListener::OnFinishLoading(UIView* uiview) =0;
	 *
	 * @brief	Executes the finish loading action.
	 *
	 * @param [in,out]	uiview	The uiview.
	 *
	 * ### param	url	URL of the document.
	 *
	 * ### param	frameName	Name of the frame.
	 */
	virtual void OnFinishLoading(UIView* uiview) =0;

	/**
	 * @fn	virtual void UIViewListener::OnCallback(UIView* uiview, const std::string& objName,
	 * 		const std::string& callbackName, const awe_jsarray* arguments) =0;
	 *
	 * @brief	Executes the callback action.
	 *
	 * @param [in,out]	uiview	The uiview.
	 * @param	objName		  	Name of the object.
	 * @param	callbackName  	Name of the callback.
	 * @param	arguments	  	The arguments.
	 */
	virtual void OnCallback(UIView* uiview, const std::string& objName, const std::string& callbackName, const awe_jsarray* arguments) =0;

	/**
	 * @fn	virtual void UIViewListener::OnChangeCursor(UIView* uiview, awe_cursor_type cursor) =0;
	 *
	 * @brief	Executes the change cursor action.
	 *
	 * @param [in,out]	uiview	The uiview.
	 * @param	cursor		  	The cursor.
	 */
	virtual void OnChangeCursor(UIView* uiview, awe_cursor_type cursor) =0;

	/**
	 * @fn	virtual void UIViewListener::OnOpenExternalLink(UIView* uiview, const std::string& url,
	 * 		std::string& sourceUrl) =0;
	 *
	 * @brief	Executes the open external link action.
	 *
	 * @param [in,out]	uiview   	The uiview.
	 * @param	url				 	URL of the document.
	 * @param [in,out]	sourceUrl	URL of the source.
	 */
	virtual void OnOpenExternalLink(UIView* uiview, const std::string& url, std::string& sourceUrl) =0;

	/**
	 * @fn	virtual void UIViewListener::OnRequestFileChooser(UIView* caller,
	 * 		bool selectMultipleFiles, const std::string& title, const std::string& defaultPath) =0;
	 *
	 * @brief	Executes the request file chooser action.
	 *
	 * @param [in,out]	caller	   	If non-null, the caller.
	 * @param	selectMultipleFiles	true to select, false to deselect the multiple files.
	 * @param	title			   	The title.
	 * @param	defaultPath		   	The default path.
	 */
	virtual void OnRequestFileChooser(UIView* caller, bool selectMultipleFiles, const std::string& title, const std::string& defaultPath) =0;
};

