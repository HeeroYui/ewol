/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <appl/debug.h>
#include <appl/MainWindows.h>

#include <ewol/widget/Button.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/Sizer.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/List.h>
#include <ewol/widget/ContextMenu.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Slider.h>
#include <ewol/widget/Menu.h>
#include <ewol/widget/meta/FileChooser.h>
#include <ewol/widget/meta/Parameter.h>
#include <ewol/widget/Select.h>
#include <ewol/widget/Manager.h>
#include <ewol/context/Context.h>
#include <appl/TestDistanceField.h>
#include <etk/os/FSNode.h>
#include <eproperty/Value.h>


static const char * l_eventChangeTheme           = "event-change-theme";
static const char * l_eventChangeWidgetNext      = "event-change-widget-test-next";
static const char * l_eventChangeWidgetPrevious  = "event-change-widget-test-previous";


#undef __class__
#define __class__ "MainWindows"

appl::MainWindows::MainWindows() :
  m_gravity(ewol::gravity_buttomLeft),
  m_idWidget(-1) {
	APPL_DEBUG("CREATE WINDOWS ... ");
	addObjectType("appl::MainWindows");
}

void appl::MainWindows::init() {
	ewol::widget::Windows::init();
	
	m_composer = ewol::widget::Composer::create(ewol::widget::Composer::file, "DATA:gui.xml");
	setSubWidget(m_composer);
	externSubBind(m_composer, ewol::widget::Button, "appl-theme-toggle", signalValue, shared_from_this(), &appl::MainWindows::onCallbackThemeChange);
	externSubBind(m_composer, ewol::widget::Button, "appl-previous-widget", signalPressed, shared_from_this(), &appl::MainWindows::onCallbackWidgetChange, -1);
	externSubBind(m_composer, ewol::widget::Button, "appl-next-widget", signalPressed, shared_from_this(), &appl::MainWindows::onCallbackWidgetChange, 1);
	externSubBind(m_composer, ewol::widget::Button, "appl-next-gravity", signalPressed, shared_from_this(), &appl::MainWindows::onCallbackGravityChange);
	
	m_sizerVert = std::dynamic_pointer_cast<ewol::widget::Sizer>(m_composer->getSubObjectNamed("appl-upper-test-widget"));
	if (m_sizerVert == nullptr) {
		APPL_CRITICAL("Can not get vertical pointer");
	}
	m_sizerDynamic = std::dynamic_pointer_cast<ewol::widget::Sizer>(m_composer->getSubObjectNamed("appl-dynamic-config"));
	if (m_sizerDynamic == nullptr) {
		APPL_CRITICAL("Can not get dynamic pointer");
	}
	m_subWidget = std::dynamic_pointer_cast<ewol::Widget>(m_composer->getSubObjectNamed("[TEST]TO-TEST"));
	if (m_subWidget == nullptr) {
		APPL_CRITICAL("Can not get subWidget pointer");
	}
}

void appl::MainWindows::onCallbackThemeChange(const bool& _value) {
	if (_value == true) {
		etk::theme::setName("GUI", "shape/round/");
	} else {
		etk::theme::setName("GUI", "shape/square/");
	}
	// Reload shaders and graphic system ...
	ewol::getContext().getResourcesManager().reLoadResources();
	ewol::getContext().forceRedrawAll();
}

void appl::MainWindows::onCallbackGravityChange() {
	switch(m_gravity) {
		case ewol::gravity_center:
			m_gravity = ewol::gravity_topLeft;
			break;
		case ewol::gravity_topLeft:
			m_gravity = ewol::gravity_top;
			break;
		case ewol::gravity_top:
			m_gravity = ewol::gravity_topRight;
			break;
		case ewol::gravity_topRight:
			m_gravity = ewol::gravity_right;
			break;
		case ewol::gravity_right:
			m_gravity = ewol::gravity_buttomRight;
			break;
		case ewol::gravity_buttomRight:
			m_gravity = ewol::gravity_buttom;
			break;
		case ewol::gravity_buttom:
			m_gravity = ewol::gravity_buttomLeft;
			break;
		case ewol::gravity_buttomLeft:
			m_gravity = ewol::gravity_left;
			break;
		case ewol::gravity_left:
			m_gravity = ewol::gravity_center;
			break;
	}
	propertySetOnWidgetNamed("appl-upper-test-widget", "gravity", ewol::gravityToString(m_gravity));
	propertySetOnWidgetNamed("appl-next-gravity-label", "value", "Next gravity<br/>(" + ewol::gravityToString(m_gravity) + ")");
}

void appl::MainWindows::onCallbackWidgetChange(int32_t _increment) {
	m_idWidget += _increment;
	std::shared_ptr<ewol::Widget> oldWidget = m_subWidget;
	std::string tmpDescription;
	std::string tmpConstruct;
	switch(m_idWidget) {
		case 0:
			tmpConstruct = "<spin/>\n";
			tmpDescription = "Test ewol::widget::Spin";
			break;
		case 1:
			tmpConstruct = std::string()
			    + "<select>\n"
			    + "    <option id='1'>plop 1</option>\n"
			    + "    <option id='2'>plop 2</option>\n"
			    + "    <option id='3' select='true'>plop 3</option>\n"
			    + "    <option id='4'>plop 4</option>\n"
			    + "    <option id='5'>plop 5</option>\n"
			    + "</select>\n";
			tmpDescription = "Test ewol::widget::Select";
			break;
		case 2:
			tmpConstruct = "<ButtonColor/>";
			tmpDescription = "Test ewol::widget::ButtonColor";
			break;
		case 3:
			tmpConstruct = "<label>Simple string</label>\n";
			tmpDescription = "Test ewol::widget::Label";
			break;
		case 4:
			tmpConstruct = "<image src='DATA:sphere.png'/>\n";
			tmpDescription = "Test ewol::widget::Image";
			break;
		case 5:
			tmpConstruct = "<checkbox><label>Simple string</label></checkbox>\n";
			tmpDescription = "Test ewol::widget::Checkbox";
			break;
		case 6:
			tmpConstruct = "<entry/>\n";
			tmpDescription = "Test ewol::widget::Entry";
			break;
		case 7:
			tmpConstruct = "<slider/>\n";
			tmpDescription = "Test ewol::widget::Entry";
			break;
		case 8:
			tmpConstruct = std::string()
			    + "<button name='[TEST]Button:TO-TEST' expand='false,false' fill='false,false' >\n"
			    + "    <label>My <font color='#FF0000'>Button</font> <br/> And Some under line<br/> plop <br/> and an other super long line ...</label>\n"
			    + "</button>\n";
			tmpDescription = "Test ewol::widget::Button";
			break;
		default:
			tmpConstruct = "<label expand=false fill=false>Simple string</label>\n";
			tmpDescription = "Test Label";
			m_idWidget = -1;
			break;
			/*
			break;
		case 1:
			m_subWidget = TestDistanceField::create();
			if (m_subWidget != nullptr) {
				m_sizerVert->subWidgetAdd(m_subWidget);
			}
			tmpDescription = "Test Distance Field";
			*/
			break;
	}
	
	m_subWidget = ewol::widget::SpinBase::create(std::unordered_map<std::string,eproperty::Variant>({{std::string("name"), eproperty::Variant(std::string("plop"))}}));
	return;
	
	
	// create the widget with a xml generator (readable for test ...):
	m_subWidget = ewol::widget::composerGenerate(ewol::widget::Composer::String, tmpConstruct);
	if (m_subWidget != nullptr) {
		m_sizerVert->subWidgetReplace(oldWidget, m_subWidget);
	}
	propertySetOnWidgetNamed("appl-label-test", "value", tmpDescription);
	updateProperty();
}

static void addSpacer(const std::shared_ptr<ewol::widget::Sizer>& _sizer, etk::Color<> _color=etk::color::none) {
	std::shared_ptr<ewol::widget::Spacer> mySpacer = ewol::widget::Spacer::create();
	if (mySpacer != nullptr) {
		mySpacer->propertyExpand.set(bvec2(true,false));
		mySpacer->propertyFill.set(bvec2(true,false));
		if (_color == etk::color::none) {
			mySpacer->propertyMinSize.set(vec2(3,3));
			mySpacer->propertyColor.set(_color);
		}
		_sizer->subWidgetAdd(mySpacer);
	}
}

void appl::MainWindows::updateProperty() {
	// remove all elements:
	m_sizerDynamic->subWidgetRemoveAll();
	if (m_subWidget == nullptr) {
		return;
	}
	std::shared_ptr<ewol::widget::Label> widget = ewol::widget::Label::create(m_subWidget->getObjectType());
	m_sizerDynamic->subWidgetAdd(widget);
	addSpacer(m_sizerDynamic, etk::color::red);
	for (size_t iii=0; iii<m_subWidget->getPropertyCount(); ++iii) {
		eproperty::Property* param = m_subWidget->getPropertyRaw(iii);
		if (param == nullptr) {
			APPL_WARNING("Parameter EMPTY . " << iii << " : nullptr");
			continue;
		}
		std::shared_ptr<ewol::widget::Sizer> widgetSizer = ewol::widget::Sizer::create(ewol::widget::Sizer::modeHori);
		if (widgetSizer != nullptr) {
			widgetSizer->propertyExpand.set(bvec2(true,false));
			widgetSizer->propertyFill.set(bvec2(true,true));
			m_sizerDynamic->subWidgetAddStart(widgetSizer);
			
			std::shared_ptr<ewol::widget::Label> widget = ewol::widget::Label::create(param->getName() + ":");
			widgetSizer->subWidgetAdd(widget);
			//addSpacer(widgetSizer, etk::color::purple);
			// Main part TODO: ...
			std::string type = param->getType();
			if (type == typeid(std::string).name()) {
				std::shared_ptr<ewol::widget::Entry> widgetTmp = ewol::widget::Entry::create();
				widgetSizer->subWidgetAdd(widgetTmp);
				eproperty::Property* param = m_subWidget->getPropertyRaw(iii);
				eproperty::Value<std::string>* paramValue = dynamic_cast<eproperty::Value<std::string>*>(param);
				if (paramValue == nullptr) {
					APPL_ERROR("nullptr...");
					return;
				}
				std::string value = paramValue->get();
				widgetTmp->propertyValue.set(value);
				widgetTmp->propertyExpand.set(bvec2(true,false));
				widgetTmp->propertyFill.set(bvec2(true,false));
				widgetTmp->signalModify.connect([=](const std::string& _value) {
						APPL_INFO("set parameter : NAME name=" << param->getName() << " value=" << _value);
						paramValue->set(_value);
						return;
					});
			} else if (type == typeid(gale::Dimension).name()) {
				type = "gale::Dimension";
			} else if (type == typeid(bvec2).name()) {
				addSpacer(widgetSizer);
				std::shared_ptr<ewol::widget::CheckBox> widgetTmp = ewol::widget::CheckBox::create();
				widgetSizer->subWidgetAdd(widgetTmp);
				eproperty::Property* param = m_subWidget->getPropertyRaw(iii);
				eproperty::Value<bvec2>* paramValue = dynamic_cast<eproperty::Value<bvec2>*>(param);
				if (paramValue == nullptr) {
					APPL_ERROR("nullptr... 2 ");
					return;
				}
				bvec2 value = paramValue->get();
				widgetTmp->propertyValue.set(value.x());
				widgetTmp->signalValue.connect([=](const bool& _value) {
						APPL_INFO("set parameter : X name=" << param->getName() << " value=" << _value);
						bvec2 lastValueInterpreted = paramValue->get();
						lastValueInterpreted.setX(_value);
						paramValue->set(lastValueInterpreted);
						return;
					});
				std::shared_ptr<ewol::widget::Label> widgetLabel = ewol::widget::Label::create("x");
				widgetTmp->setSubWidget(widgetLabel);
				
				widgetTmp = ewol::widget::CheckBox::create();
				widgetSizer->subWidgetAdd(widgetTmp);
				widgetTmp->propertyValue.set(value.y());
				widgetTmp->signalValue.connect([=](const bool& _value) {
						APPL_INFO("set parameter : Y name=" << param->getName() << " value=" << _value);
						bvec2 lastValueInterpreted = paramValue->get();
						lastValueInterpreted.setY(_value);
						paramValue->set(lastValueInterpreted);
						return;
					});
				widgetLabel = ewol::widget::Label::create("y");
				widgetTmp->setSubWidget(widgetLabel);
			} else if (type == typeid(ivec2).name()) {
				type = "ivec2";
			} else if (type == typeid(uivec2).name()) {
				type = "uivec2";
			} else if (type == typeid(vec2).name()) {
				type = "vec2";
			} else if (type == typeid(bool).name()) {
				addSpacer(widgetSizer);
				std::shared_ptr<ewol::widget::CheckBox> widgetTmp = ewol::widget::CheckBox::create();
				widgetSizer->subWidgetAdd(widgetTmp);
				widgetTmp->signalValue.connect([=](const bool& _value) {
						if (m_subWidget == nullptr) {
							APPL_ERROR("nullptr...");
							return;
						}
						APPL_INFO("set parameter : name=" << param->getName() << " value=" << _value);
						m_subWidget->propertySet(param->getName(), etk::to_string(_value));
						return;
					});
			} else if (    type == typeid(int64_t).name()
			            || type == typeid(int32_t).name()
			            || type == typeid(int16_t).name()
			            || type == typeid(int8_t).name()
			            || type == typeid(uint64_t).name()
			            || type == typeid(uint32_t).name()
			            || type == typeid(uint16_t).name()
			            || type == typeid(uint8_t).name()) {
				std::shared_ptr<ewol::widget::Entry> widgetTmp = ewol::widget::Entry::create();
				widgetSizer->subWidgetAdd(widgetTmp);
				eproperty::Property* param = m_subWidget->getPropertyRaw(iii);
				std::string value = param->getString();
				widgetTmp->propertyValue.set(value);
				widgetTmp->propertyExpand.set(bvec2(true,false));
				widgetTmp->propertyFill.set(bvec2(true,false));
				widgetTmp->signalModify.connect([=](const std::string& _value) {
						APPL_INFO("set parameter : NAME name=" << param->getName() << " value=" << _value);
						param->setString(_value);
						return;
					});
				if (type == typeid(int64_t).name()) {
					type = "int64_t";
				} else if (type == typeid(int32_t).name()) {
					type = "int32_t";
				} else if (type == typeid(int16_t).name()) {
					type = "int16_t";
				} else if (type == typeid(int8_t).name()) {
					type = "int8_t";
				} else if (type == typeid(uint64_t).name()) {
					type = "uint64_t";
				} else if (type == typeid(uint32_t).name()) {
					type = "uint32_t";
				} else if (type == typeid(uint16_t).name()) {
					type = "uint16_t";
				} else if (type == typeid(uint8_t).name()) {
					type = "uint8_t";
				}
			} else if (type == typeid(float).name()) {
				type = "float";
			} else if (type == typeid(double).name()) {
				type = "double";
			} else if (type == typeid(enum ewol::gravity).name()) {
				type = "enum ewol::gravity";
				std::shared_ptr<ewol::widget::Select> widgetTmp = ewol::widget::Select::create();
				widgetSizer->subWidgetAdd(widgetTmp);
				widgetTmp->propertyExpand.set(bvec2(true,false));
				widgetTmp->propertyFill.set(bvec2(true,false));
				widgetTmp->optionAdd(int32_t(ewol::gravity_center), "Center");
				widgetTmp->optionAdd(int32_t(ewol::gravity_top), "Top");
				widgetTmp->optionAdd(int32_t(ewol::gravity_buttom), "Buttom");
				widgetTmp->optionAdd(int32_t(ewol::gravity_right), "Right");
				widgetTmp->optionAdd(int32_t(ewol::gravity_left), "Left");
				widgetTmp->optionAdd(int32_t(ewol::gravity_topRight), "Top-right");
				widgetTmp->optionAdd(int32_t(ewol::gravity_topLeft), "Top-left");
				widgetTmp->optionAdd(int32_t(ewol::gravity_buttomRight), "Buttom-right");
				widgetTmp->optionAdd(int32_t(ewol::gravity_buttomLeft), "Buttom-left");
				eproperty::Property* param = m_subWidget->getPropertyRaw(iii);
				eproperty::List<ewol::gravity>* paramValue = dynamic_cast<eproperty::List<ewol::gravity>*>(param);
				if (paramValue == nullptr) {
					APPL_ERROR("nullptr... 2 ");
					return;
				}
				ewol::gravity value = paramValue->get();
				widgetTmp->propertyValue.set(value);
				widgetTmp->signalValue.connect([=](const int32_t& _value) {
					enum ewol::gravity val = ewol::gravity(_value);
					APPL_INFO("set parameter: gravity name=" << param->getName() << " value=" << val);
					paramValue->set(val);
					return;
				});
			}
		}
		std::shared_ptr<ewol::widget::Spacer> mySpacer = ewol::widget::Spacer::create();
		if (mySpacer != nullptr) {
			mySpacer->propertyExpand.set(bvec2(true,false));
			mySpacer->propertyFill.set(bvec2(true,false));
			mySpacer->propertyMinSize.set(vec2(3,3));
			mySpacer->propertyColor.set(etk::color::blue);
			m_sizerDynamic->subWidgetAddStart(mySpacer);
		}
	}
}

