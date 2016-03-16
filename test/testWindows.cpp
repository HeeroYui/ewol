/**
 * @author Edouard DUPIN
 * 
 * @copyright 2015, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <test-debug/debug.h>
#include <ewol/widget/Windows.h>
#include <gtest/gtest.h>
#define NAME "Windows"

#undef __class__
#define __class__	"TestEwolWindows"

namespace appl {
	class Windows : public ewol::widget::Windows {
		protected:
			Windows() {
				addObjectType("appl::Windows");
				propertyTitle.setDirectCheck("test set");
			}
			void init() {
				ewol::widget::Windows::init();
			}
		public:
			DECLARE_FACTORY(Windows);
	};
};




TEST(TestEwolWindows, Creation) {
	/*
	std::shared_ptr<appl::Windows> tmpWindows = appl::Windows::create();
	EXPECT_NE(tmpWindows, nullptr);
	tmpWindows.reset();
	*/
}

