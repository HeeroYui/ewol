/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/widget/TreeView.hpp>
#include <etk/os/FSNode.hpp>
#include <ewol/resource/ColorFile.hpp>
#include <esignal/Signal.hpp>
#include <etk/FlatTree.hpp>

namespace appl {
	namespace widget {
		class BasicTree;
		using BasicTreeShared = ememory::SharedPtr<appl::widget::BasicTree>;
		using BasicTreeWeak = ememory::WeakPtr<appl::widget::BasicTree>;
		class TreeElement {
			public:
				TreeElement(const etk::String& _display="", bool _isSelected = false, bool m_isExpand=false):
				  m_display(_display),
				  m_isSelected(_isSelected),
				  m_isExpand(_isExpand) {
					
				}
				etk::String m_display;
				bool m_isSelected = false;
				bool m_isExpand = false;
		}
		/**
		 * @brief Generic display folder class. This widget display the content of a single folder :
		 */
		class BasicTree : public ewol::widget::TreeView {
			protected:
				BasicTree();
			public:
				DECLARE_WIDGET_FACTORY(BasicTree, "BasicTree");
				virtual ~BasicTree();
			protected:
				etk::Color<> getBasicBG() override;
				ivec2 getMatrixSize() const override;
				fluorine::Variant getData(int32_t _role, const ivec2& _pos) override;
				bool onItemEvent(int32_t _IdInput, enum gale::key::status _typeEvent, const ivec2& _pos, const vec2& _mousePosition) override;
				
				ememory::SharedPtr<etk::TreeNode<NodeElement>> m_tree;
				etk::FlatTree<TreeElement> m_flatTree;
		};
	};
};


