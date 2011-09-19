#pragma once

#include <iostream>

typedef struct _awe_jsarray awe_jsarray;
typedef enum _awe_cursor_type awe_cursor_type;
class UIView;

class UIViewListener
{
public:
	/**
	 * @fn	virtual void UIViewListener::OnBeginNavigation(const UIView* uiview,
	 * 		const std::string& url, const std::string& frameName) =0;
	 *
	 * @brief	Executes the begin navigation action.
	 *
	 * @param	uiview   	The uiview.
	 * @param	url		 	URL of the document.
	 * @param	frameName	Name of the frame.
	 */
	virtual void OnBeginNavigation(const UIView* uiview, const std::string& url, const std::string& frameName) =0;

	/**
	 * @fn	virtual void UIViewListener::OnBeginLoading(const UIView* uiview, const std::string& url,
	 * 		const std::string& frameName) =0;
	 *
	 * @brief	Executes the begin loading action.
	 *
	 * @param	uiview   	The uiview.
	 * @param	url		 	URL of the document.
	 * @param	frameName	Name of the frame.
	 */
	virtual void OnBeginLoading(const UIView* uiview, const std::string& url, const std::string& frameName, int status_code, const std::string& mime_type) =0;

	/**
	 * @fn	virtual void UIViewListener::OnFinishLoading(const UIView* uiview, const std::string& url,
	 * 		const std::string& frameName) =0;
	 *
	 * @brief	Executes the finish loading action.
	 *
	 * @param	uiview   	The uiview.
	 * @param	url		 	URL of the document.
	 * @param	frameName	Name of the frame.
	 */
	virtual void OnFinishLoading(const UIView* uiview) =0;

	/**
	 * @fn	virtual void UIViewListener::OnCallback(const UIView* uiview, const std::string& objName,
	 * 		const std::string& callbackName, const awe_jsarray* arguments) =0;
	 *
	 * @brief	Executes the callback action.
	 *
	 * @param	uiview			The uiview.
	 * @param	objName			Name of the object.
	 * @param	callbackName	Name of the callback.
	 * @param	arguments   	The arguments.
	 */
	virtual void OnCallback(const UIView* uiview, const std::string& objName, const std::string& callbackName, const awe_jsarray* arguments) =0;

	/**
	 * @fn	virtual void UIViewListener::OnChangeCursor(const UIView* uiview,
	 * 		awe_cursor_type cursor) =0;
	 *
	 * @brief	Executes the change cursor action.
	 *
	 * @param	uiview	The uiview.
	 * @param	cursor	The cursor.
	 */
	virtual void OnChangeCursor(const UIView* uiview, awe_cursor_type cursor) =0;

	/**
	 * @fn	virtual void UIViewListener::OnOpenExternalLink(const UIView* uiview,
	 * 		const std::string& url, std::string& sourceUrl) =0;
	 *
	 * @brief	Executes the open external link action.
	 *
	 * @param	uiview			 	The uiview.
	 * @param	url				 	URL of the document.
	 * @param [in,out]	sourceUrl	URL of the source.
	 */
	virtual void OnOpenExternalLink(const UIView* uiview, const std::string& url, std::string& sourceUrl) =0;
};

