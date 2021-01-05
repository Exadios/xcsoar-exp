KOBO_MENU_SOURCES = \
	$(SRC)/Version.cpp \
	$(SRC)/Asset.cpp \
	$(SRC)/Formatter/HexColor.cpp \
	$(SRC)/Hardware/CPU.cpp \
	$(SRC)/Hardware/DisplayDPI.cpp \
	$(SRC)/Hardware/DisplaySize.cpp \
	$(SRC)/Hardware/RotateDisplay.cpp \
	$(SRC)/Screen/Layout.cpp \
	$(SRC)/Screen/TerminalWindow.cpp \
	$(SRC)/Look/TerminalLook.cpp \
	$(SRC)/Look/DialogLook.cpp \
	$(SRC)/Look/ButtonLook.cpp \
	$(SRC)/Look/CheckBoxLook.cpp \
	$(SRC)/Gauge/LogoView.cpp \
	$(SRC)/Dialogs/DialogSettings.cpp \
	$(SRC)/Dialogs/WidgetDialog.cpp \
	$(SRC)/Dialogs/HelpDialog.cpp \
	$(SRC)/Dialogs/Message.cpp \
	$(SRC)/Dialogs/TextEntry.cpp \
	$(SRC)/Dialogs/KnobTextEntry.cpp \
	$(SRC)/Dialogs/TouchTextEntry.cpp \
	$(SRC)/Dialogs/SimulatorPromptWindow.cpp \
	$(TEST_SRC_DIR)/Fonts.cpp \
	$(TEST_SRC_DIR)/FakeLanguage.cpp \
	$(SRC)/Kobo/WPASupplicant.cpp \
	$(SRC)/Kobo/Model.cpp \
	$(SRC)/Kobo/System.cpp \
	$(SRC)/Kobo/Kernel.cpp \
	$(SRC)/Kobo/NetworkDialog.cpp \
	$(SRC)/Kobo/SystemDialog.cpp \
	$(SRC)/Kobo/ToolsDialog.cpp \
	$(SRC)/Kobo/WPASupplicant.cpp \
	$(SRC)/Kobo/WifiDialog.cpp \
	$(SRC)/Kobo/FakeSymbols.cpp \
	$(SRC)/Kobo/KoboMenu.cpp
KOBO_MENU_LDADD = $(FAKE_LIBS)
KOBO_MENU_DEPENDS = WIDGET FORM SCREEN EVENT RESOURCE IO ASYNC LIBNET OS THREAD MATH UTIL
KOBO_MENU_STRIP = y

$(eval $(call link-program,KoboMenu,KOBO_MENU))

ifeq ($(TARGET),UNIX)
OPTIONAL_OUTPUTS += $(KOBO_MENU_BIN)
endif

ifeq ($(TARGET_IS_KOBO),y)

KOBO_POWER_OFF_SOURCES = \
	$(TEST_SRC_DIR)/Fonts.cpp \
	$(SRC)/Hardware/RotateDisplay.cpp \
	$(SRC)/Hardware/DisplayDPI.cpp \
	$(SRC)/Hardware/DisplaySize.cpp \
	$(SRC)/Screen/Layout.cpp \
	$(SRC)/Logger/FlightParser.cpp \
	$(SRC)/Renderer/FlightListRenderer.cpp \
	$(SRC)/Thread/Mutex.cpp \
	$(SRC)/FlightInfo.cpp \
	$(SRC)/Kobo/Model.cpp \
	$(SRC)/Kobo/PowerOff.cpp
KOBO_POWER_OFF_LDADD = $(FAKE_LIBS)
KOBO_POWER_OFF_DEPENDS = SCREEN EVENT RESOURCE IO ASYNC OS MATH UTIL TIME
KOBO_POWER_OFF_STRIP = y
$(eval $(call link-program,PowerOff,KOBO_POWER_OFF))
OPTIONAL_OUTPUTS += $(KOBO_POWER_OFF_BIN)

BITSTREAM_VERA_DIR ?= $(shell \
if [ -d /usr/share/fonts/truetype/ttf-bitstream-vera ]; then \
	echo /usr/share/fonts/truetype/ttf-bitstream-vera; \
elif [ -d /usr/share/fonts/bitstream-vera ]; then \
	echo /usr/share/fonts/bitstream-vera; \
fi)
BITSTREAM_VERA_NAMES = Vera VeraBd VeraIt VeraBI VeraMono
BITSTREAM_VERA_FILES = $(patsubst %,$(BITSTREAM_VERA_DIR)/%.ttf,$(BITSTREAM_VERA_NAMES))

KOBO_KERNEL_DIR = /opt/kobo/kernel

# /mnt/onboard/.kobo/KoboRoot.tgz is a file that is picked up by
# /etc/init.d/rcS, extracted to / on each boot; we can use it to
# install XCSoar
$(TARGET_OUTPUT_DIR)/KoboRoot.tgz: $(XCSOAR_BIN) \
	$(KOBO_MENU_BIN) $(KOBO_POWER_OFF_BIN) \
	$(BITSTREAM_VERA_FILES) \
	$(topdir)/kobo/etc/inittab \
  $(topdir)/kobo/etc/inetd.conf \
  $(topdir)/kobo/bin/rcS \
  $(topdir)/kobo/bin/postinit.sh \
  $(topdir)/kobo/bin/usb2dsd.sh \
  $(topdir)/kobo/bin/usb2eth.sh \
  $(topdir)/kobo/etc/udev/ac \
  $(topdir)/kobo/etc/udev/plug
	@$(NQ)echo "  TAR     $@"
	$(Q)rm -rf $(@D)/KoboRoot
	$(Q)install -m 0755 -d $(@D)/KoboRoot/etc $(@D)/KoboRoot/etc/udev $(@D)/KoboRoot/opt/xcsoar/bin $(@D)/KoboRoot/opt/xcsoar/lib/kernel $(@D)/KoboRoot/opt/xcsoar/share/fonts
	$(Q)install -m 0755 $(XCSOAR_BIN) $(KOBO_MENU_BIN) $(KOBO_POWER_OFF_BIN) $(topdir)/kobo/bin/rcS $(@D)/KoboRoot/opt/xcsoar/bin
	$(Q)install -m 0755 $(topdir)/kobo/bin/postinit.sh $(@D)/KoboRoot/opt/xcsoar/bin
	$(Q)install -m 0755 $(topdir)/kobo/opt/afterinit.sh $(@D)/KoboRoot/opt
	$(Q)install -m 0755 $(topdir)/kobo/opt/inetd.conf $(@D)/KoboRoot/opt
	$(Q)install -m 0755 $(topdir)/kobo/bin/usb2dsd.sh  $(@D)/KoboRoot/opt/xcsoar/bin
	$(Q)install -m 0755 $(topdir)/kobo/bin/usb2eth.sh  $(@D)/KoboRoot/opt/xcsoar/bin
	$(Q)if test -f $(KOBO_KERNEL_DIR)/uImage.kobo; then install -m 0644 $(KOBO_KERNEL_DIR)/uImage.kobo $(@D)/KoboRoot/opt/xcsoar/lib/kernel; fi
	$(Q)if test -f $(KOBO_KERNEL_DIR)/uImage.otg; then install -m 0644 $(KOBO_KERNEL_DIR)/uImage.otg $(@D)/KoboRoot/opt/xcsoar/lib/kernel; fi
	$(Q)if test -f $(KOBO_KERNEL_DIR)/uImage.glohd; then install -m 0644 $(KOBO_KERNEL_DIR)/uImage.glohd $(@D)/KoboRoot/opt/xcsoar/lib/kernel; fi
	$(Q)if test -f $(KOBO_KERNEL_DIR)/uImage.glohd.otg; then install -m 0644 $(KOBO_KERNEL_DIR)/uImage.glohd.otg $(@D)/KoboRoot/opt/xcsoar/lib/kernel; fi
	$(Q)if test -f $(KOBO_KERNEL_DIR)/uImage.aura2; then install -m 0644 $(KOBO_KERNEL_DIR)/uImage.aura2 $(@D)/KoboRoot/opt/xcsoar/lib/kernel; fi
	$(Q)if test -f $(KOBO_KERNEL_DIR)/uImage.aura2.otg; then install -m 0644 $(KOBO_KERNEL_DIR)/uImage.aura2.otg $(@D)/KoboRoot/opt/xcsoar/lib/kernel; fi
#	$(Q)if test -f $(KOBO_KERNEL_DIR)/zImage.N249; then install -m 0644 $(KOBO_KERNEL_DIR)/zImage.N249 $(@D)/KoboRoot/opt/xcsoar/lib/kernel; fi
	$(Q)install -m 0644 $(topdir)/kobo/etc/inittab    $(@D)/KoboRoot/etc
	$(Q)install -m 0644 $(topdir)/kobo/etc/inetd.conf $(@D)/KoboRoot/etc
	$(Q)install -m 0755 $(topdir)/kobo/etc/udev/ac    $(@D)/KoboRoot/etc/udev
	$(Q)install -m 0755 $(topdir)/kobo/etc/udev/plug  $(@D)/KoboRoot/etc/udev

	$(Q)install -m 0644 $(BITSTREAM_VERA_FILES) $(@D)/KoboRoot/opt/xcsoar/share/fonts
	$(Q)fakeroot tar czfC $@ $(@D)/KoboRoot .

endif
