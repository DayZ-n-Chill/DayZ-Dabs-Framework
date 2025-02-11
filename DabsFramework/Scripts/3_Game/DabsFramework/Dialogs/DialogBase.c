class DialogBase: ScriptView
{
	// Private members
	protected DialogResult m_DialogResult = DialogResult.None;

	// ViewController
	protected DialogBaseController m_DialogBaseController;
	
	// View Properties	
	protected WrapSpacerWidget WindowDragWrapper;
	protected ImageWidget TitleIcon;
	
	void DialogBase(string title)
	{
		m_DialogBaseController = DialogBaseController.Cast(GetController());
		
		m_DialogBaseController.Title = title;
		m_DialogBaseController.NotifyPropertyChanged("Title");
		
		m_DialogBaseController.Icon = GetIcon();
		if (m_DialogBaseController.Icon != string.Empty) {
			m_DialogBaseController.NotifyPropertyChanged("Icon");
			TitleIcon.Show(true);
		}
		
		m_LayoutRoot.Show(false);
	}
			
	DialogResult ShowDialog()
	{
		Trace("ShowDialog");
		m_LayoutRoot.Show(true);
		while (m_DialogResult == DialogResult.None) {
			Sleep(10);
		}
		
		Log("DialogResult: %1", typename.EnumToString(DialogResult, m_DialogResult));		
		return m_DialogResult;
	}
	
	void CloseDialog(DialogResult dialog_result = DialogResult.Cancel)
	{
		Trace("CloseDialog");
		m_DialogResult = dialog_result;
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Delete, 15);
	}
	
	void ExitDialogExecute()
	{	
		CloseDialog(DialogResult.Cancel);
	}
	
	private void Delete()
	{
		delete this;
	}
		
	ScriptView AddContent(ScriptView content)
	{
		content.SetParent(this);
		m_DialogBaseController.DialogContentData.Insert(content);
		return content;
	}
	
	DialogButton AddButton(DialogResult result)
	{
		return AddButton(GetDialogResultText(result), result);
	}
	
	DialogButton AddButton(string label, string callback)
	{
		return AddButton(new DialogButton(label, callback));
	}
	
	DialogButton AddButton(string label, DialogResult result)
	{
		return AddButton(new DialogExitButton(label, "DialogExitButtonCallback", result));
	}
		
	DialogButton AddButton(DialogButton button)
	{
		button.SetParent(this);
		m_DialogBaseController.DialogButtonData.Insert(button);
		return button;
	}
	
	static string GetDialogResultText(DialogResult result)
	{
		// TODO: Temporary, until stringtables are complete
		return typename.EnumToString(DialogResult, result);
		
		switch (result) {
			case DialogResult.None:		return "#STR_DIALOG_NONE";
			case DialogResult.OK: 		return "#STR_DIALOG_OK";
			case DialogResult.Cancel: 	return "#STR_DIALOG_CANCEL";
			case DialogResult.Abort:	return "#STR_DIALOG_ABORT";
			case DialogResult.Retry:	return "#STR_DIALOG_RETRY";
			case DialogResult.Ignore:	return "#STR_DIALOG_IGNORE";
			case DialogResult.Yes:		return "#STR_DIALOG_YES";
			case DialogResult.No:		return "#STR_DIALOG_NO";
		}
		
		return typename.EnumToString(DialogResult, result);
	}
	
	protected void DialogExitButtonCallback(DialogExitButton button)
	{
		CloseDialog(button.ButtonResult);
	}
	
	private float m_OffsetX, m_OffsetY;
	override bool OnDrag(Widget w, int x, int y)
	{
		Trace("OnDrag");
		if (w == WindowDragWrapper) {
			m_LayoutRoot.GetPos(m_OffsetX, m_OffsetY);
			m_OffsetX -= x; m_OffsetY -= y;		
			g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Insert(DragUpdate);	
		}
		
		return false;
	}
	
	override bool OnDragging(Widget w, int x, int y, Widget reciever) 
	{ 
		return true; 
	}
	
	private bool DragUpdate()
	{
		int x, y;
		GetMousePos(x, y);
		m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);
		return false;
	}
	
	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		Trace("OnDrop");
		g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Remove(DragUpdate);
		
		if (w == WindowDragWrapper) {
			m_LayoutRoot.SetPos(x + m_OffsetX, y + m_OffsetY);
	    }		
		
		return false;
	}
	
	// Abstract
	string GetIcon()
	{
		return string.Empty;
	}
	
	override typename GetControllerType() 
	{
		return DialogBaseController;
	}	
		
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/dialogs/DialogBase.layout";
	}
}