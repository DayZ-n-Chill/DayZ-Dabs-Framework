class EditBoxNumberPrefab: PrefabBase<StringEvaluater>
{
	EditBoxWidget ContentText;
	
	protected float m_StepSize;
	
	void EditBoxNumberPrefab(string caption, Class binding_context, string binding_name, float step_size = 1)
	{
		m_StepSize = step_size;
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		float motion = wheel;
		motion *= 0.5;
		motion *= m_StepSize;
		
		if (KeyState(KeyCode.KC_LCONTROL)) {
			motion *= 10;
		}
		
		if (KeyState(KeyCode.KC_LSHIFT)) {
			motion *= 0.1;
		}
		
		switch (w.GetName()) {
			
			case "ContentText": {
				m_PrefabBaseController.Value = string.ToString(m_PrefabBaseController.Value.Parse() + motion);
				m_PrefabBaseController.NotifyPropertyChanged("Value");
				break;
			}
		}
		
		return false;
	}
	
	override StringEvaluater GetDefaultValue(Class binding_context, string binding_name)
	{
		float value;
		EnScript.GetClassVar(binding_context, binding_name, 0, value);
		return value.ToString();
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
		EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, m_PrefabBaseController.Value.Parse());
		g_Script.CallFunction(m_BindingContext, "PropertyChanged", null, m_BindingName);
	}
	
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/EditBoxPrefab.layout";
	}
}