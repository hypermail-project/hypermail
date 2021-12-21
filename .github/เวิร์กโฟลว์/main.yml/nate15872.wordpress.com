# This is a basic workflow to help you get started with Actions

name: CI

# Controls when the workflow will run
on:
  # Triggers the workflow on push or pull request events but only for the master branch
  push:
    branches: [ master <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<rss version="2.0">
  <!-- Note to users: this is the RSS2 compliant form of 
lkml.org/rdf.php . See the comments over there too.
-->
  <channel>
    <title>lkml.org</title>
    <link>http://lkml.org</link>
    <description>lkml.org - the realtime linux kernel mailinglist archive</description>
    <lastBuildDate>Tue, 21 Dec 2021 03:31:16 +0100</lastBuildDate>
    <item>
      <title>Re: [PATCH] psi: fix possible trigger missing in the window</title>
      <author>Suren Baghdasaryan &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1493</link>
      <description>Suren Baghdasaryan writes: (Summary) 
We fire the trigger only if growth within a given window is higher
than the threshold.&lt;br/&gt;
than the threshold.&lt;br/&gt;
In your scenario if the stall recorded in the 1st polling_min_period
was less than the threshold and in the other 9 polling_min_periods no
new stalls were registered then there should be no triggers fired in
that window.
</description>
    </item>
    <item>
      <title>[PATCH v2] ext4: fix fast commit may miss tracking range for FALLO ...</title>
      <author>Xin Yin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1492</link>
      <description>Xin Yin writes: (Summary)  If the range is align to blocksize,
fast commit will not track range for this change.&lt;br/&gt;
fast commit will not track range for this change.&lt;br/&gt;
Also track range for unwritten range in ext4_map_blocks(). inode-&amp;gt;i_sb-&amp;gt;s_blocksize_bits,
-   (offset + len - 1) &amp;gt;&amp;gt;
   }
-  ext4_fc_track_range(handle, inode, map-&amp;gt;m_lblk,
-       map-&amp;gt;m_lblk + map-&amp;gt;m_len - 1); EXT4_MAP_MAPPED))
+  ext4_fc_track_range(handle, inode, map-&amp;gt;m_lblk,
+     map-&amp;gt;m_lblk + map-&amp;gt;m_len - 1);
</description>
    </item>
    <item>
      <title>[PATCH -next] drm/i915/gvt: Fix intel_vgpu_default_mmio_write() ke ...</title>
      <author>Yang Li &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1491</link>
      <description>Yang Li writes: (Summary)  2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/drivers/gpu/drm/i915/gvt/handlers.c b/drivers/gpu/drm/i915/gvt/handlers.c
index 3938df0db188..32027a7e08af 100644
--- a/drivers/gpu/drm/i915/gvt/handlers.c
+++ b/drivers/gpu/drm/i915/gvt/handlers.c
@@ -3798,7 +3798,7 @@ int intel_vgpu_default_mmio_read(struct intel_vgpu *vgpu, unsigned int offset,
 }
 
 /**
- * intel_t_default_mmio_write - default MMIO write handler
+ * intel_vgpu_default_mmio_write - default MMIO write handler
  * @vgpu: a vGPU
  * @offset: access offset
  * @p_data: write data buffer
-- 
2.20.1.7.g153144c
&lt;br/&gt;

</description>
    </item>
    <item>
      <title>Re: [PATCH 1/4] x86/entry: Make paranoid_exit() callable</title>
      <author>Lai Jiangshan &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1490</link>
      <description>Lai Jiangshan writes: (Summary)   The
changelog of it in the original big patchset reads:
changelog of it in the original big patchset reads:
   Allow paranoid_exit() to be re-written in C later and also allow
   asm_exc_nmi() to call it to avoid duplicated code.&lt;br/&gt;
   asm_exc_nmi() to call it to avoid duplicated code.&lt;br/&gt;
And this smaller patchset doesn't include the work of converting ASM code,
so I removed "Allow paranoid_exit() to be re-written in C later" because I
thought "allowing asm_exc_nmi() to call it and avoiding duplicated code" is
enough to justify the value of this change.&lt;br/&gt;
enough to justify the value of this change.&lt;br/&gt;
When paranoid_exit() is ready to be converted to C, it can't have jump to
any label that is not in paranoid_exit() itself.&lt;br/&gt;
any label that is not in paranoid_exit() itself.&lt;br/&gt;
I'm sorry to not have put all of the intents in the changelog:
I'm sorry to not have put all of the intents in the changelog:
-It will allow asm_exc_nmi() to call it and avoid duplicated code.
</description>
    </item>
    <item>
      <title>[PATCH -next] usb: early: Fix an error code in xdbc_init()</title>
      <author>Yang Li &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1489</link>
      <description>Yang Li writes: (Summary) When xdbc_trace("hardware not used anymore\n") is performed,
ret should be assigned -ENODEV to indicate this.&lt;br/&gt;
ret should be assigned -ENODEV to indicate this.&lt;br/&gt;
Clean up smatch warning:&lt;br/&gt;
drivers/usb/early/xhci-dbc.c:972 xdbc_init() warn: missing error code
'ret'&lt;br/&gt;
'ret'&lt;br/&gt;
Reported-by: Abaci Robot &amp;lt;abaci@linux.alibaba.com&amp;gt; 1 +
 1 file changed, 1 insertion(+)

diff --git a/drivers/usb/early/xhci-dbc.c b/drivers/usb/early/xhci-dbc.c
index 4502108069cd..fcb05a8948fa 100644
--- a/drivers/usb/early/xhci-dbc.c
+++ b/drivers/usb/early/xhci-dbc.c
@@ -969,6 +969,7 @@ static int __init xdbc_init(void)
  if (early_xdbc_console.index == -1 ||
</description>
    </item>
    <item>
      <title>[PATCH] exec: Make suid_dumpable apply to SUID/SGID binaries irres ...</title>
      <author>Waiman Long &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1488</link>
      <description>Waiman Long writes: (Summary)  5 ++++-
 2 files changed, 7 insertions(+), 4 deletions(-)

diff --git a/fs/exec.c b/fs/exec.c
index 537d92c41105..60e02e678fb6 100644
--- a/fs/exec.c
+++ b/fs/exec.c
@@ -1344,9 +1344,7 @@ int begin_new_exec(struct linux_binprm * bprm)
   * is wrong, but userspace depends on it.
  }
 }
 
diff --git a/include/linux/binfmts.h b/include/linux/binfmts.h
index 049cf9421d83..6d9893c59085 100644
--- a/include/linux/binfmts.h
+++ b/include/linux/binfmts.h
@@ -41,7 +41,10 @@ struct linux_binprm {
    * Set when errors can no longer be returned to the
    * original userspace.
</description>
    </item>
    <item>
      <title>Re: [PATCH V4 2/2] PCI: vmd: Override ASPM on TGL/ADL VMD devices</title>
      <author>Rajat Jain &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1487</link>
      <description>Rajat Jain writes: (Summary)   You say below that there's an M.2 slot, so
&amp;gt; build a platform with a VMD root port connected to a regular PCIe x4
&amp;gt; +
&amp;gt; +           pdev-&amp;gt;class !=3D PCI_CLASS_STORAGE_EXPRESS)
&amp;gt; From what I've seen these root ports
&amp;gt; think this value should be in your .driver_data (which would mean
&amp;gt; The current code suggests that 0x1003 is the correct value for *all*
&amp;gt; value would be the correct value for all possible hierarchies and
&amp;gt;
</description>
    </item>
    <item>
      <title>Re: [PATCH v5 00/10] add aspeed-jpeg support for aspeed-video</title>
      <author>Jammy Huang &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1486</link>
      <description>Jammy Huang writes: (Summary) Hi Sakari,&lt;br/&gt;
Hi Sakari,&lt;br/&gt;
On 2021/12/21 &amp;#19978;&amp;#21320;
In order to have aspeed-jpeg format to work on openbmc's KVM, I was 
working on noVNC.&lt;br/&gt;
And this codec library is part of the code to make it work. So I check&lt;br/&gt;
what noVNC used, &lt;a href="https://github.com/novnc/noVNC/blob/master/LICENSE.txt."&gt;https://github.com/novnc/noVNC/blob/master/LICENSE.txt.&lt;/a&gt; 
That's why I&lt;br/&gt;
adapted MPL 2.0.&lt;br/&gt;
adapted MPL 2.0.&lt;br/&gt;
If you think GPL(or LGPL) is more suitable, I can make a switch.
If you think GPL(or LGPL) is more suitable, I can make a switch.
</description>
    </item>
    <item>
      <title>Re: [PATCH v2 1/1] ata: libahci_platform: Get rid of dup message w ...</title>
      <author>Damien Le Moal &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1485</link>
      <description>Damien Le Moal writes: On 12/17/21 20:28, Andy Shevchenko wrote:&lt;br/&gt;

 		return -EINVAL;&lt;br/&gt;
Applied to for-5.17. Thanks !&lt;br/&gt;
Applied to for-5.17. Thanks !&lt;br/&gt;

</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 08/14] net: bcmgenet: Fix NULL vs IS_ERR() checking</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1484</link>
      <description>Sasha Levin writes: (Summary) From: Miaoqian Lin &amp;lt;linmq006@gmail.com&amp;gt;&lt;br/&gt;
From: Miaoqian Lin &amp;lt;linmq006@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit ab8eb798ddabddb2944401bf31ead9671cb97d95 ]
[ Upstream commit ab8eb798ddabddb2944401bf31ead9671cb97d95 ]
The phy_attach() function does not return NULL. 4 ++--
 1 file changed, 2 insertions(+), 2 deletions(-)

diff --git a/drivers/net/ethernet/broadcom/genet/bcmmii.c b/drivers/net/ethernet/broadcom/genet/bcmmii.c
index ce569b7d3b353..e494f0e57d8ff 100644
--- a/drivers/net/ethernet/broadcom/genet/bcmmii.c
+++ b/drivers/net/ethernet/broadcom/genet/bcmmii.c
@@ -547,9 +547,9 @@ static int bcmgenet_mii_pd_init(struct bcmgenet_priv *priv)
    * Internal or external PHY with MDIO access
    */
   phydev = phy_attach(priv-&amp;gt;dev, phy_name, pd-&amp;gt;phy_interface);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 10/14] mac80211: update channel context before  ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1483</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 4dde3c3627b52ca515a34f6f4de3898224aa1dd3 ]
[ Upstream commit 4dde3c3627b52ca515a34f6f4de3898224aa1dd3 ]
Currently channel context is updated only after station got an update about
new assoc state, this results in station using the old channel context. 15 +++++++++------
 1 file changed, 9 insertions(+), 6 deletions(-)

diff --git a/net/mac80211/sta_info.c b/net/mac80211/sta_info.c
index 384a95617ee5e..6a1d3935bd3e1 100644
--- a/net/mac80211/sta_info.c
+++ b/net/mac80211/sta_info.c
@@ -651,6 +651,15 @@ static int sta_info_insert_finish(struct sta_info *sta) __acquires(RCU)
 
  list_add_tail_rcu(&amp;amp;sta-&amp;gt;list, &amp;amp;local-&amp;gt;sta_list);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 15/19] block: reduce kblockd_mod_delayed_work_ ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1482</link>
      <description>Sasha Levin writes: (Summary) From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
[ Upstream commit cb2ac2912a9ca7d3d26291c511939a41361d2d83 ]
[ Upstream commit cb2ac2912a9ca7d3d26291c511939a41361d2d83 ]
Dexuan reports that he's seeing spikes of very heavy CPU utilization when
running 24 disks and using the 'none' scheduler. This happens off the
sched restart path, because SCSI requires the queue to be restarted async,
and hence we're hammering on mod_delayed_work_on() to ensure that the work
item gets run appropriately.&lt;br/&gt;
item gets run appropriately.&lt;br/&gt;
Avoid hammering on the timer and just use queue_work_on() if no delay
has been specified.&lt;br/&gt;
has been specified.&lt;br/&gt;
Reported-and-tested-by: Dexuan Cui &amp;lt;decui@microsoft.com&amp;gt;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 12/14] block: reduce kblockd_mod_delayed_work_o ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1481</link>
      <description>Sasha Levin writes: (Summary) From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
[ Upstream commit cb2ac2912a9ca7d3d26291c511939a41361d2d83 ]
[ Upstream commit cb2ac2912a9ca7d3d26291c511939a41361d2d83 ]
Dexuan reports that he's seeing spikes of very heavy CPU utilization when
running 24 disks and using the 'none' scheduler. This happens off the
sched restart path, because SCSI requires the queue to be restarted async,
and hence we're hammering on mod_delayed_work_on() to ensure that the work
item gets run appropriately.&lt;br/&gt;
item gets run appropriately.&lt;br/&gt;
Avoid hammering on the timer and just use queue_work_on() if no delay
has been specified.&lt;br/&gt;
has been specified.&lt;br/&gt;
Reported-and-tested-by: Dexuan Cui &amp;lt;decui@microsoft.com&amp;gt;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 16/19] net: usb: lan78xx: add Allied Telesis A ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1480</link>
      <description>Sasha Levin writes: (Summary)  6 ++++++
 1 file changed, 6 insertions(+)

diff --git a/drivers/net/usb/lan78xx.c b/drivers/net/usb/lan78xx.c
index c666e990900b9..6f7b70522d926 100644
--- a/drivers/net/usb/lan78xx.c
+++ b/drivers/net/usb/lan78xx.c
@@ -64,6 +64,8 @@
 #define LAN7801_USB_PRODUCT_ID  (0x7801)
 #define LAN78XX_EEPROM_MAGIC  (0x78A5)
 #define LAN78XX_OTP_MAGIC  (0x78F3)
+#define AT29M2AF_USB_VENDOR_ID  (0x07C9)
+#define AT29M2AF_USB_PRODUCT_ID (0x0012)
 
 #define MII_READ   1
 #define MII_WRITE   0
@@ -4142,6 +4144,10 @@ static const struct usb_device_id products[] = {
  /* LAN7801 USB Gigabit Ethernet Device */
  USB_DEVICE(LAN78XX_USB_VENDOR_ID, LAN7801_USB_PRODUCT_ID),
  },
+ {
+ /* ATM2-AF USB Gigabit Ethernet Device */
+ USB_DEVICE(AT29M2AF_USB_VENDOR_ID, AT29M2AF_USB_PRODUCT_ID),
+ },
  {},
 };
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 09/14] mac80211: Fix the size used for building ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1479</link>
      <description>Sasha Levin writes: (Summary) From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
Instead of using the hard-coded value of '100' use the correct
scan IEs length as calculated during HW registration to mac80211. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/net/mac80211/util.c b/net/mac80211/util.c
index decd46b383938..fc326a3fc2e64 100644
--- a/net/mac80211/util.c
+++ b/net/mac80211/util.c
@@ -1901,7 +1901,7 @@ struct sk_buff *ieee80211_build_probe_req(struct ieee80211_sub_if_data *sdata,
   chandef.chan = chan;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 14/19] drm/amdgpu: correct the wrong cached st ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1478</link>
      <description>Sasha Levin writes: (Summary) From: Evan Quan &amp;lt;evan.quan@amd.com&amp;gt;&lt;br/&gt;
From: Evan Quan &amp;lt;evan.quan@amd.com&amp;gt;&lt;br/&gt;
[ Upstream commit 17c65d6fca844ee72a651944d8ce721e9040bf70 ]
[ Upstream commit 17c65d6fca844ee72a651944d8ce721e9040bf70 ]
Pair the operations did in GMC -&amp;gt;hw_init and -&amp;gt;hw_fini.
  }
 
+ /*
+  * Pair the operations did in gmc_v9_0_hw_init and thus maintain
+  * a correct cached state for GMC. AMD_PG_SUPPORT_MMHUB)
+  amdgpu_dpm_set_powergating_by_smu(adev,
+        AMD_IP_BLOCK_TYPE_GMC,
+        enable);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.19 08/11] mac80211: do drv_reconfig_complete() be ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1477</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 13dee10b30c058ee2c58c5da00339cc0d4201aa6 ]
[ Upstream commit 13dee10b30c058ee2c58c5da00339cc0d4201aa6 ]
When we reconfigure, the driver might do some things to complete
the reconfiguration. 14 +++++++-------
 1 file changed, 7 insertions(+), 7 deletions(-)

diff --git a/net/mac80211/util.c b/net/mac80211/util.c
index a653632ac5902..7eda8b59e4da8 100644
--- a/net/mac80211/util.c
+++ b/net/mac80211/util.c
@@ -2222,6 +2222,13 @@ int ieee80211_reconfig(struct ieee80211_local *local)
   mutex_unlock(&amp;amp;local-&amp;gt;sta_mtx);
 
- /*
-  * If this is for hw restart things are still running.
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 13/14] net: usb: lan78xx: add Allied Telesis AT ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1476</link>
      <description>Sasha Levin writes: (Summary)  6 ++++++
 1 file changed, 6 insertions(+)

diff --git a/drivers/net/usb/lan78xx.c b/drivers/net/usb/lan78xx.c
index 2d98373f7a71d..ce3c8f476d75c 100644
--- a/drivers/net/usb/lan78xx.c
+++ b/drivers/net/usb/lan78xx.c
@@ -64,6 +64,8 @@
 #define LAN7801_USB_PRODUCT_ID  (0x7801)
 #define LAN78XX_EEPROM_MAGIC  (0x78A5)
 #define LAN78XX_OTP_MAGIC  (0x78F3)
+#define AT29M2AF_USB_VENDOR_ID  (0x07C9)
+#define AT29M2AF_USB_PRODUCT_ID (0x0012)
 
 #define MII_READ   1
 #define MII_WRITE   0
@@ -4153,6 +4155,10 @@ static const struct usb_device_id products[] = {
  /* LAN7801 USB Gigabit Ethernet Device */
  USB_DEVICE(LAN78XX_USB_VENDOR_ID, LAN7801_USB_PRODUCT_ID),
  },
+ {
+ /* ATM2-AF USB Gigabit Ethernet Device */
+ USB_DEVICE(AT29M2AF_USB_VENDOR_ID, AT29M2AF_USB_PRODUCT_ID),
+ },
  {},
 };
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 11/14] mac80211: do drv_reconfig_complete() bef ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1475</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 13dee10b30c058ee2c58c5da00339cc0d4201aa6 ]
[ Upstream commit 13dee10b30c058ee2c58c5da00339cc0d4201aa6 ]
When we reconfigure, the driver might do some things to complete
the reconfiguration. 14 +++++++-------
 1 file changed, 7 insertions(+), 7 deletions(-)

diff --git a/net/mac80211/util.c b/net/mac80211/util.c
index fc326a3fc2e64..73a9e21b060d6 100644
--- a/net/mac80211/util.c
+++ b/net/mac80211/util.c
@@ -2486,6 +2486,13 @@ int ieee80211_reconfig(struct ieee80211_local *local)
   mutex_unlock(&amp;amp;local-&amp;gt;sta_mtx);
 
- /*
-  * If this is for hw restart things are still running.
</description>
    </item>
    <item>
      <title>Re: [PATCH] init: fix the wrong __setup_param() definition</title>
      <author>yajun.deng@linux ... &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1474</link>
      <description>   yajun.deng@linux ... writes: (Summary)  =0A&amp;gt;&amp;gt; I don't object thi=
s fix, since the !MODULE __setup_param() seems like=0A&amp;gt; But for the current code, it's really a bug.=0Avim drivers/clk/im=
x/clk.c  +161=0A=0A#ifndef MODULE=0A=0A...=0A__setup_param("earlycon", im=
x_keep_uart_earlycon,=0A              imx_keep_uart_clocks_param, 0); =0A&amp;gt;&amp;gt; =0A&amp;gt;&amp;gt; =0A&amp;gt;&amp;gt; =0A&amp;gt;&amp;gt; -#define __setup_param(str, unique_id, fn) /* nothing */=
=0A&amp;gt;&amp;gt; +#define __setup_param(str, unique_id, fn, early)/* nothing */=0A&amp;gt;&amp;gt;=
 #define __setup(str, func) /* nothing */=0A&amp;gt;&amp;gt; =0A&amp;gt;&amp;gt;
</description>
    </item>
    <item>
      <title>Regarding Of My Late Father's Fund $10,200,000,</title>
      <author>"Miss.Mmamie Shimirah" &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1473</link>
      <description>&amp;quot;Miss.Mmamie Shimirah&amp;quot; writes: (Summary)  During the war my father
made a lot of money through the illegal sales of Diamonds to the tune
of $10,200,000.&lt;br/&gt;
of $10,200,000.&lt;br/&gt;
This money is currently and secretly kept in ECOWAS security company
in Porto-Novo City of Benin Republic, but because of the political
turmoil which still&lt;br/&gt;
exists in Africa, I can not invest the money by myself, hence am
soliciting your help to help me take these funds into your custody for
invest.&lt;br/&gt;
invest.&lt;br/&gt;
I want to add here that if agreed 35% that's $3,570,000 of the total
worth of the fund will be yours minus your total expenses incurred
during the clearing of the fund in Porto Novo Benin.
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.9 4/4] mac80211: Fix the size used for building p ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1472</link>
      <description>Sasha Levin writes: (Summary) From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
Instead of using the hard-coded value of '100' use the correct
scan IEs length as calculated during HW registration to mac80211. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/net/mac80211/util.c b/net/mac80211/util.c
index 52f9742c438a4..a7e1a29a95933 100644
--- a/net/mac80211/util.c
+++ b/net/mac80211/util.c
@@ -1545,7 +1545,7 @@ struct sk_buff *ieee80211_build_probe_req(struct ieee80211_sub_if_data *sdata,
   chandef.chan = chan;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.14 9/9] Revert "block: reduce kblockd_mod_delayed ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1471</link>
      <description>Sasha Levin writes: (Summary) From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
[ Upstream commit 87959fa16cfbcf76245c11559db1940069621274 ]
[ Upstream commit 87959fa16cfbcf76245c11559db1940069621274 ]
This reverts commit cb2ac2912a9ca7d3d26291c511939a41361d2d83. 2 --
 1 file changed, 2 deletions(-)

diff --git a/block/blk-core.c b/block/blk-core.c
index 1859490fa4ae1..2407c898ba7d8 100644
--- a/block/blk-core.c
+++ b/block/blk-core.c
@@ -3233,8 +3233,6 @@ EXPORT_SYMBOL(kblockd_schedule_work_on);
 int kblockd_mod_delayed_work_on(int cpu, struct delayed_work *dwork,
     unsigned long delay)
 {
- if (!delay)
-  return queue_work_on(cpu, kblockd_workqueue, &amp;amp;dwork-&amp;gt;work);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.14 8/9] net: usb: lan78xx: add Allied Telesis AT2 ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1470</link>
      <description>Sasha Levin writes: (Summary)  6 ++++++
 1 file changed, 6 insertions(+)

diff --git a/drivers/net/usb/lan78xx.c b/drivers/net/usb/lan78xx.c
index 838b6fe9dfaaf..dc8afd5575fe2 100644
--- a/drivers/net/usb/lan78xx.c
+++ b/drivers/net/usb/lan78xx.c
@@ -74,6 +74,8 @@
 #define LAN7801_USB_PRODUCT_ID  (0x7801)
 #define LAN78XX_EEPROM_MAGIC  (0x78A5)
 #define LAN78XX_OTP_MAGIC  (0x78F3)
+#define AT29M2AF_USB_VENDOR_ID  (0x07C9)
+#define AT29M2AF_USB_PRODUCT_ID (0x0012)
 
 #define MII_READ   1
 #define MII_WRITE   0
@@ -4015,6 +4017,10 @@ static const struct usb_device_id products[] = {
  /* LAN7801 USB Gigabit Ethernet Device */
  USB_DEVICE(LAN78XX_USB_VENDOR_ID, LAN7801_USB_PRODUCT_ID),
  },
+ {
+ /* ATM2-AF USB Gigabit Ethernet Device */
+ USB_DEVICE(AT29M2AF_USB_VENDOR_ID, AT29M2AF_USB_PRODUCT_ID),
+ },
  {},
 };
</description>
    </item>
    <item>
      <title>[PATCH]  fs/squashfs: handle possible null pointer</title>
      <author>Peng Hao &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1469</link>
      <description>Peng Hao writes: (Summary)  in squashfs_fill_super:&lt;br/&gt;
 in squashfs_fill_super:&lt;br/&gt;
        msblk-&amp;gt;decompressor = supported_squashfs_filesystem(
                        fc,&lt;br/&gt;
                        le16_to_cpu(sblk-&amp;gt;s_major),&lt;br/&gt;
                        le16_to_cpu(sblk-&amp;gt;s_minor),&lt;br/&gt;
                        le16_to_cpu(sblk-&amp;gt;compression));&lt;br/&gt;
        if (msblk-&amp;gt;decompressor == NULL)
                goto failed_mount;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.4 3/4] phonet: refcount leak in pep_sock_accep</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1468</link>
      <description>Sasha Levin writes: (Summary) From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
sock_hold(sk) is invoked in pep_sock_accept(), but __sock_put(sk) is not
invoked in subsequent failure branches(pep_accept_conn() != 0). 1 +
 1 file changed, 1 insertion(+)

diff --git a/net/phonet/pep.c b/net/phonet/pep.c
index f6aa532bcbf64..65883d1b9ec72 100644
--- a/net/phonet/pep.c
+++ b/net/phonet/pep.c
@@ -878,6 +878,7 @@ static struct sock *pep_sock_accept(struct sock *sk, int flags, int *errp)
 
  err = pep_accept_conn(newsk, skb);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.4 4/4] mac80211: Fix the size used for building p ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1467</link>
      <description>Sasha Levin writes: (Summary) From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
Instead of using the hard-coded value of '100' use the correct
scan IEs length as calculated during HW registration to mac80211. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/net/mac80211/util.c b/net/mac80211/util.c
index 4301a92fc160f..ede948de83c72 100644
--- a/net/mac80211/util.c
+++ b/net/mac80211/util.c
@@ -1540,7 +1540,7 @@ struct sk_buff *ieee80211_build_probe_req(struct ieee80211_sub_if_data *sdata,
   chandef.chan = chan;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.14 6/9] mac80211: Fix the size used for building  ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1466</link>
      <description>Sasha Levin writes: (Summary) From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
Instead of using the hard-coded value of '100' use the correct
scan IEs length as calculated during HW registration to mac80211. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/net/mac80211/util.c b/net/mac80211/util.c
index cd3cdd1a0b576..e25d570479bf6 100644
--- a/net/mac80211/util.c
+++ b/net/mac80211/util.c
@@ -1548,7 +1548,7 @@ struct sk_buff *ieee80211_build_probe_req(struct ieee80211_sub_if_data *sdata,
   chandef.chan = chan;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.14 4/9] phonet: refcount leak in pep_sock_accep</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1465</link>
      <description>Sasha Levin writes: (Summary) From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
sock_hold(sk) is invoked in pep_sock_accept(), but __sock_put(sk) is not
invoked in subsequent failure branches(pep_accept_conn() != 0). 1 +
 1 file changed, 1 insertion(+)

diff --git a/net/phonet/pep.c b/net/phonet/pep.c
index bffcef58ebf5c..f1bdd4f1b1b43 100644
--- a/net/phonet/pep.c
+++ b/net/phonet/pep.c
@@ -881,6 +881,7 @@ static struct sock *pep_sock_accept(struct sock *sk, int flags, int *errp,
 
  err = pep_accept_conn(newsk, skb);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.14 1/9] ARM: rockchip: Use memcpy_toio instead of ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1464</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
This fixes a potential kernel panic on memcpy when FORTIFY_SOURCE
is enabled. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/arch/arm/mach-rockchip/platsmp.c b/arch/arm/mach-rockchip/platsmp.c
index ecec340ca3457..bb69e6e6defe8 100644
--- a/arch/arm/mach-rockchip/platsmp.c
+++ b/arch/arm/mach-rockchip/platsmp.c
@@ -198,7 +198,7 @@ static int __init rockchip_smp_prepare_sram(struct device_node *node)
  rockchip_boot_fn = __pa_symbol(secondary_startup);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.9 3/4] phonet: refcount leak in pep_sock_accep</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1463</link>
      <description>Sasha Levin writes: (Summary) From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
sock_hold(sk) is invoked in pep_sock_accept(), but __sock_put(sk) is not
invoked in subsequent failure branches(pep_accept_conn() != 0). 1 +
 1 file changed, 1 insertion(+)

diff --git a/net/phonet/pep.c b/net/phonet/pep.c
index f6aa532bcbf64..65883d1b9ec72 100644
--- a/net/phonet/pep.c
+++ b/net/phonet/pep.c
@@ -878,6 +878,7 @@ static struct sock *pep_sock_accept(struct sock *sk, int flags, int *errp)
 
  err = pep_accept_conn(newsk, skb);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.19 05/11] net: bcmgenet: Fix NULL vs IS_ERR() che ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1462</link>
      <description>Sasha Levin writes: (Summary) From: Miaoqian Lin &amp;lt;linmq006@gmail.com&amp;gt;&lt;br/&gt;
From: Miaoqian Lin &amp;lt;linmq006@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit ab8eb798ddabddb2944401bf31ead9671cb97d95 ]
[ Upstream commit ab8eb798ddabddb2944401bf31ead9671cb97d95 ]
The phy_attach() function does not return NULL. 4 ++--
 1 file changed, 2 insertions(+), 2 deletions(-)

diff --git a/drivers/net/ethernet/broadcom/genet/bcmmii.c b/drivers/net/ethernet/broadcom/genet/bcmmii.c
index 494601c39b847..0b6f6a3fd7347 100644
--- a/drivers/net/ethernet/broadcom/genet/bcmmii.c
+++ b/drivers/net/ethernet/broadcom/genet/bcmmii.c
@@ -549,9 +549,9 @@ static int bcmgenet_mii_pd_init(struct bcmgenet_priv *priv)
    * Internal or external PHY with MDIO access
    */
   phydev = phy_attach(priv-&amp;gt;dev, phy_name, pd-&amp;gt;phy_interface);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.4 2/4] mac80211: set up the fwd_skb-&gt;dev for mesh ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1461</link>
      <description>Sasha Levin writes: (Summary) 
[   72.962020 ] CPU: 0 PID: 1193 Comm: kworker/u5:1 Tainted: P S 5.4.137 #0
[   72.969938 ] Hardware name: MT7622_MT7531 RFB (DT)
[   72.974659 ] Workqueue: napi_workq napi_workfn&lt;br/&gt;
[   72.979025 ] pstate: 60000005 (nZCv daif -PAN -UAO)
[   72.983822 ] pc : __skb_flow_dissect+0x284/0x1298
[   72.988444 ] lr : __skb_flow_dissect+0x54/0x1298
[   72.992977 ] sp : ffffffc010c738c0
[   72.996293 ] x29: ffffffc010c738c0 x28: 0000000000000000
[   73.001615 ] x27: 000000000000ffc2 x26: ffffff800c2eb818
[   73.006937 ] x25: ffffffc010a987c8 x24: 00000000000000ce
[   73.012259 ] x23: ffffffc010c73a28 x22: ffffffc010a99c60
[   73.017581 ] x21: 000000000000ffc2 x20: ffffff80094da800
[   73.022903 ] x19: 0000000000000000 x18: 0000000000000014
[   73.028226 ] x17: 00000000084d16af x16: 00000000d1fc0bab
[   73.033548 ] x15: 00000000715f6034 x14: 000000009dbdd301
[   73.038870 ] x13: 00000000ea4dcbc3 x12: 0000000000000040
[   73.044192 ] x11: 000000000eb00ff0 x10: 0000000000000000
[   73.049513 ] x9 : 000000000eb000</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.4 1/4] ARM: rockchip: Use memcpy_toio instead of  ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1460</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
This fixes a potential kernel panic on memcpy when FORTIFY_SOURCE
is enabled. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/arch/arm/mach-rockchip/platsmp.c b/arch/arm/mach-rockchip/platsmp.c
index 3e7a4b761a953..47b6739937c64 100644
--- a/arch/arm/mach-rockchip/platsmp.c
+++ b/arch/arm/mach-rockchip/platsmp.c
@@ -194,7 +194,7 @@ static int __init rockchip_smp_prepare_sram(struct device_node *node)
  rockchip_boot_fn = virt_to_phys(secondary_startup);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.9 2/4] mac80211: set up the fwd_skb-&gt;dev for mesh ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1459</link>
      <description>Sasha Levin writes: (Summary) 
[   72.962020 ] CPU: 0 PID: 1193 Comm: kworker/u5:1 Tainted: P S 5.4.137 #0
[   72.969938 ] Hardware name: MT7622_MT7531 RFB (DT)
[   72.974659 ] Workqueue: napi_workq napi_workfn&lt;br/&gt;
[   72.979025 ] pstate: 60000005 (nZCv daif -PAN -UAO)
[   72.983822 ] pc : __skb_flow_dissect+0x284/0x1298
[   72.988444 ] lr : __skb_flow_dissect+0x54/0x1298
[   72.992977 ] sp : ffffffc010c738c0
[   72.996293 ] x29: ffffffc010c738c0 x28: 0000000000000000
[   73.001615 ] x27: 000000000000ffc2 x26: ffffff800c2eb818
[   73.006937 ] x25: ffffffc010a987c8 x24: 00000000000000ce
[   73.012259 ] x23: ffffffc010c73a28 x22: ffffffc010a99c60
[   73.017581 ] x21: 000000000000ffc2 x20: ffffff80094da800
[   73.022903 ] x19: 0000000000000000 x18: 0000000000000014
[   73.028226 ] x17: 00000000084d16af x16: 00000000d1fc0bab
[   73.033548 ] x15: 00000000715f6034 x14: 000000009dbdd301
[   73.038870 ] x13: 00000000ea4dcbc3 x12: 0000000000000040
[   73.044192 ] x11: 000000000eb00ff0 x10: 0000000000000000
[   73.049513 ] x9 : 000000000eb000</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.9 1/4] ARM: rockchip: Use memcpy_toio instead of  ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1458</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
This fixes a potential kernel panic on memcpy when FORTIFY_SOURCE
is enabled. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/arch/arm/mach-rockchip/platsmp.c b/arch/arm/mach-rockchip/platsmp.c
index 4d827a069d49c..203f5a72829e2 100644
--- a/arch/arm/mach-rockchip/platsmp.c
+++ b/arch/arm/mach-rockchip/platsmp.c
@@ -198,7 +198,7 @@ static int __init rockchip_smp_prepare_sram(struct device_node *node)
  rockchip_boot_fn = virt_to_phys(secondary_startup);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.19 04/11] phonet: refcount leak in pep_sock_accep</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1457</link>
      <description>Sasha Levin writes: (Summary) From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
sock_hold(sk) is invoked in pep_sock_accept(), but __sock_put(sk) is not
invoked in subsequent failure branches(pep_accept_conn() != 0). 1 +
 1 file changed, 1 insertion(+)

diff --git a/net/phonet/pep.c b/net/phonet/pep.c
index db34735403035..63d34e702df52 100644
--- a/net/phonet/pep.c
+++ b/net/phonet/pep.c
@@ -881,6 +881,7 @@ static struct sock *pep_sock_accept(struct sock *sk, int flags, int *errp,
 
  err = pep_accept_conn(newsk, skb);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.14 5/9] net: bcmgenet: Fix NULL vs IS_ERR() checking</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1456</link>
      <description>Sasha Levin writes: (Summary) From: Miaoqian Lin &amp;lt;linmq006@gmail.com&amp;gt;&lt;br/&gt;
From: Miaoqian Lin &amp;lt;linmq006@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit ab8eb798ddabddb2944401bf31ead9671cb97d95 ]
[ Upstream commit ab8eb798ddabddb2944401bf31ead9671cb97d95 ]
The phy_attach() function does not return NULL. 4 ++--
 1 file changed, 2 insertions(+), 2 deletions(-)

diff --git a/drivers/net/ethernet/broadcom/genet/bcmmii.c b/drivers/net/ethernet/broadcom/genet/bcmmii.c
index 72fad2a63c625..e609154d1ac93 100644
--- a/drivers/net/ethernet/broadcom/genet/bcmmii.c
+++ b/drivers/net/ethernet/broadcom/genet/bcmmii.c
@@ -535,9 +535,9 @@ static int bcmgenet_mii_pd_init(struct bcmgenet_priv *priv)
    * Internal or external PHY with MDIO access
    */
   phydev = phy_attach(priv-&amp;gt;dev, phy_name, pd-&amp;gt;phy_interface);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.14 7/9] block: reduce kblockd_mod_delayed_work_on ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1455</link>
      <description>Sasha Levin writes: (Summary) From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
[ Upstream commit cb2ac2912a9ca7d3d26291c511939a41361d2d83 ]
[ Upstream commit cb2ac2912a9ca7d3d26291c511939a41361d2d83 ]
Dexuan reports that he's seeing spikes of very heavy CPU utilization when
running 24 disks and using the 'none' scheduler. This happens off the
sched restart path, because SCSI requires the queue to be restarted async,
and hence we're hammering on mod_delayed_work_on() to ensure that the work
item gets run appropriately.&lt;br/&gt;
item gets run appropriately.&lt;br/&gt;
Avoid hammering on the timer and just use queue_work_on() if no delay
has been specified.&lt;br/&gt;
has been specified.&lt;br/&gt;
Reported-and-tested-by: Dexuan Cui &amp;lt;decui@microsoft.com&amp;gt;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.14 3/9] mac80211: fix a memory leak where sta_inf ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1454</link>
      <description>Sasha Levin writes: (Summary) From: Ahmed Zaki &amp;lt;anzaki@gmail.com&amp;gt;&lt;br/&gt;
From: Ahmed Zaki &amp;lt;anzaki@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit 8f9dcc29566626f683843ccac6113a12208315ca ]
[ Upstream commit 8f9dcc29566626f683843ccac6113a12208315ca ]
The following is from a system that went OOM due to a memory leak:
The following is from a system that went OOM due to a memory leak:
wlan0: Allocated STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: Allocated STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: IBSS finish 74:83:c2:64:0b:87 (---from ieee80211_ibss_add_sta)
wlan0: Adding new IBSS station 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 2&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 3&lt;br/&gt;
wlan0: Inserted STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: IBSS finish 74:83:c2:64:0b:87 (---from ieee80211_ibss_work)
wlan0: Adding new IBSS station 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 2&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 3&lt;br/&gt;
.&lt;br/&gt;
.&lt;br/&gt;
wlan0: expiring inactive not authorized STA 74:83:c2:64:0b:87
wlan0: moving </description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.14 2/9] mac80211: set up the fwd_skb-&gt;dev for mes ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1453</link>
      <description>Sasha Levin writes: (Summary) 
[   72.962020 ] CPU: 0 PID: 1193 Comm: kworker/u5:1 Tainted: P S 5.4.137 #0
[   72.969938 ] Hardware name: MT7622_MT7531 RFB (DT)
[   72.974659 ] Workqueue: napi_workq napi_workfn&lt;br/&gt;
[   72.979025 ] pstate: 60000005 (nZCv daif -PAN -UAO)
[   72.983822 ] pc : __skb_flow_dissect+0x284/0x1298
[   72.988444 ] lr : __skb_flow_dissect+0x54/0x1298
[   72.992977 ] sp : ffffffc010c738c0
[   72.996293 ] x29: ffffffc010c738c0 x28: 0000000000000000
[   73.001615 ] x27: 000000000000ffc2 x26: ffffff800c2eb818
[   73.006937 ] x25: ffffffc010a987c8 x24: 00000000000000ce
[   73.012259 ] x23: ffffffc010c73a28 x22: ffffffc010a99c60
[   73.017581 ] x21: 000000000000ffc2 x20: ffffff80094da800
[   73.022903 ] x19: 0000000000000000 x18: 0000000000000014
[   73.028226 ] x17: 00000000084d16af x16: 00000000d1fc0bab
[   73.033548 ] x15: 00000000715f6034 x14: 000000009dbdd301
[   73.038870 ] x13: 00000000ea4dcbc3 x12: 0000000000000040
[   73.044192 ] x11: 000000000eb00ff0 x10: 0000000000000000
[   73.049513 ] x9 : 000000000eb000</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.19 02/11] mac80211: set up the fwd_skb-&gt;dev for m ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1452</link>
      <description>Sasha Levin writes: (Summary) 
[   72.962020 ] CPU: 0 PID: 1193 Comm: kworker/u5:1 Tainted: P S 5.4.137 #0
[   72.969938 ] Hardware name: MT7622_MT7531 RFB (DT)
[   72.974659 ] Workqueue: napi_workq napi_workfn&lt;br/&gt;
[   72.979025 ] pstate: 60000005 (nZCv daif -PAN -UAO)
[   72.983822 ] pc : __skb_flow_dissect+0x284/0x1298
[   72.988444 ] lr : __skb_flow_dissect+0x54/0x1298
[   72.992977 ] sp : ffffffc010c738c0
[   72.996293 ] x29: ffffffc010c738c0 x28: 0000000000000000
[   73.001615 ] x27: 000000000000ffc2 x26: ffffff800c2eb818
[   73.006937 ] x25: ffffffc010a987c8 x24: 00000000000000ce
[   73.012259 ] x23: ffffffc010c73a28 x22: ffffffc010a99c60
[   73.017581 ] x21: 000000000000ffc2 x20: ffffff80094da800
[   73.022903 ] x19: 0000000000000000 x18: 0000000000000014
[   73.028226 ] x17: 00000000084d16af x16: 00000000d1fc0bab
[   73.033548 ] x15: 00000000715f6034 x14: 000000009dbdd301
[   73.038870 ] x13: 00000000ea4dcbc3 x12: 0000000000000040
[   73.044192 ] x11: 000000000eb00ff0 x10: 0000000000000000
[   73.049513 ] x9 : 000000000eb000</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.19 11/11] Revert "block: reduce kblockd_mod_delay ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1451</link>
      <description>Sasha Levin writes: (Summary) From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
[ Upstream commit 87959fa16cfbcf76245c11559db1940069621274 ]
[ Upstream commit 87959fa16cfbcf76245c11559db1940069621274 ]
This reverts commit cb2ac2912a9ca7d3d26291c511939a41361d2d83. 2 --
 1 file changed, 2 deletions(-)

diff --git a/block/blk-core.c b/block/blk-core.c
index 8529cc3f213b9..80f3e729fdd4d 100644
--- a/block/blk-core.c
+++ b/block/blk-core.c
@@ -3581,8 +3581,6 @@ EXPORT_SYMBOL(kblockd_schedule_work_on);
 int kblockd_mod_delayed_work_on(int cpu, struct delayed_work *dwork,
     unsigned long delay)
 {
- if (!delay)
-  return queue_work_on(cpu, kblockd_workqueue, &amp;amp;dwork-&amp;gt;work);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.19 10/11] net: usb: lan78xx: add Allied Telesis A ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1450</link>
      <description>Sasha Levin writes: (Summary)  6 ++++++
 1 file changed, 6 insertions(+)

diff --git a/drivers/net/usb/lan78xx.c b/drivers/net/usb/lan78xx.c
index b328207c04555..6e5988f23077e 100644
--- a/drivers/net/usb/lan78xx.c
+++ b/drivers/net/usb/lan78xx.c
@@ -75,6 +75,8 @@
 #define LAN7801_USB_PRODUCT_ID  (0x7801)
 #define LAN78XX_EEPROM_MAGIC  (0x78A5)
 #define LAN78XX_OTP_MAGIC  (0x78F3)
+#define AT29M2AF_USB_VENDOR_ID  (0x07C9)
+#define AT29M2AF_USB_PRODUCT_ID (0x0012)
 
 #define MII_READ   1
 #define MII_WRITE   0
@@ -4172,6 +4174,10 @@ static const struct usb_device_id products[] = {
  /* LAN7801 USB Gigabit Ethernet Device */
  USB_DEVICE(LAN78XX_USB_VENDOR_ID, LAN7801_USB_PRODUCT_ID),
  },
+ {
+ /* ATM2-AF USB Gigabit Ethernet Device */
+ USB_DEVICE(AT29M2AF_USB_VENDOR_ID, AT29M2AF_USB_PRODUCT_ID),
+ },
  {},
 };
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.19 09/11] block: reduce kblockd_mod_delayed_work_ ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1449</link>
      <description>Sasha Levin writes: (Summary) From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
[ Upstream commit cb2ac2912a9ca7d3d26291c511939a41361d2d83 ]
[ Upstream commit cb2ac2912a9ca7d3d26291c511939a41361d2d83 ]
Dexuan reports that he's seeing spikes of very heavy CPU utilization when
running 24 disks and using the 'none' scheduler. This happens off the
sched restart path, because SCSI requires the queue to be restarted async,
and hence we're hammering on mod_delayed_work_on() to ensure that the work
item gets run appropriately.&lt;br/&gt;
item gets run appropriately.&lt;br/&gt;
Avoid hammering on the timer and just use queue_work_on() if no delay
has been specified.&lt;br/&gt;
has been specified.&lt;br/&gt;
Reported-and-tested-by: Dexuan Cui &amp;lt;decui@microsoft.com&amp;gt;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.19 07/11] mac80211: update channel context before ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1448</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 4dde3c3627b52ca515a34f6f4de3898224aa1dd3 ]
[ Upstream commit 4dde3c3627b52ca515a34f6f4de3898224aa1dd3 ]
Currently channel context is updated only after station got an update about
new assoc state, this results in station using the old channel context. 15 +++++++++------
 1 file changed, 9 insertions(+), 6 deletions(-)

diff --git a/net/mac80211/sta_info.c b/net/mac80211/sta_info.c
index 03f4358020796..663aecf76765f 100644
--- a/net/mac80211/sta_info.c
+++ b/net/mac80211/sta_info.c
@@ -597,6 +597,15 @@ static int sta_info_insert_finish(struct sta_info *sta) __acquires(RCU)
 
  list_add_tail_rcu(&amp;amp;sta-&amp;gt;list, &amp;amp;local-&amp;gt;sta_list);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.19 06/11] mac80211: Fix the size used for buildin ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1447</link>
      <description>Sasha Levin writes: (Summary) From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
Instead of using the hard-coded value of '100' use the correct
scan IEs length as calculated during HW registration to mac80211. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/net/mac80211/util.c b/net/mac80211/util.c
index 7fa9871b1db9f..a653632ac5902 100644
--- a/net/mac80211/util.c
+++ b/net/mac80211/util.c
@@ -1638,7 +1638,7 @@ struct sk_buff *ieee80211_build_probe_req(struct ieee80211_sub_if_data *sdata,
   chandef.chan = chan;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.19 03/11] mac80211: fix a memory leak where sta_i ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1446</link>
      <description>Sasha Levin writes: (Summary) From: Ahmed Zaki &amp;lt;anzaki@gmail.com&amp;gt;&lt;br/&gt;
From: Ahmed Zaki &amp;lt;anzaki@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit 8f9dcc29566626f683843ccac6113a12208315ca ]
[ Upstream commit 8f9dcc29566626f683843ccac6113a12208315ca ]
The following is from a system that went OOM due to a memory leak:
The following is from a system that went OOM due to a memory leak:
wlan0: Allocated STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: Allocated STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: IBSS finish 74:83:c2:64:0b:87 (---from ieee80211_ibss_add_sta)
wlan0: Adding new IBSS station 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 2&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 3&lt;br/&gt;
wlan0: Inserted STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: IBSS finish 74:83:c2:64:0b:87 (---from ieee80211_ibss_work)
wlan0: Adding new IBSS station 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 2&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 3&lt;br/&gt;
.&lt;br/&gt;
.&lt;br/&gt;
wlan0: expiring inactive not authorized STA 74:83:c2:64:0b:87
wlan0: moving </description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 4.19 01/11] ARM: rockchip: Use memcpy_toio instead  ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1445</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
This fixes a potential kernel panic on memcpy when FORTIFY_SOURCE
is enabled. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/arch/arm/mach-rockchip/platsmp.c b/arch/arm/mach-rockchip/platsmp.c
index 51984a40b097f..fb233a8f0bf54 100644
--- a/arch/arm/mach-rockchip/platsmp.c
+++ b/arch/arm/mach-rockchip/platsmp.c
@@ -198,7 +198,7 @@ static int __init rockchip_smp_prepare_sram(struct device_node *node)
  rockchip_boot_fn = __pa_symbol(secondary_startup);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 14/14] Revert "block: reduce kblockd_mod_delaye ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1444</link>
      <description>Sasha Levin writes: (Summary) From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
[ Upstream commit 87959fa16cfbcf76245c11559db1940069621274 ]
[ Upstream commit 87959fa16cfbcf76245c11559db1940069621274 ]
This reverts commit cb2ac2912a9ca7d3d26291c511939a41361d2d83. 2 --
 1 file changed, 2 deletions(-)

diff --git a/block/blk-core.c b/block/blk-core.c
index 78b7a21cf1d69..5808baa950c35 100644
--- a/block/blk-core.c
+++ b/block/blk-core.c
@@ -1669,8 +1669,6 @@ EXPORT_SYMBOL(kblockd_schedule_work_on);
 int kblockd_mod_delayed_work_on(int cpu, struct delayed_work *dwork,
     unsigned long delay)
 {
- if (!delay)
-  return queue_work_on(cpu, kblockd_workqueue, &amp;amp;dwork-&amp;gt;work);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 07/14] phonet: refcount leak in pep_sock_accep</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1443</link>
      <description>Sasha Levin writes: (Summary) From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
sock_hold(sk) is invoked in pep_sock_accept(), but __sock_put(sk) is not
invoked in subsequent failure branches(pep_accept_conn() != 0). 1 +
 1 file changed, 1 insertion(+)

diff --git a/net/phonet/pep.c b/net/phonet/pep.c
index 4577e43cb7778..ea3b480ce20d9 100644
--- a/net/phonet/pep.c
+++ b/net/phonet/pep.c
@@ -868,6 +868,7 @@ static struct sock *pep_sock_accept(struct sock *sk, int flags, int *errp,
 
  err = pep_accept_conn(newsk, skb);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 04/14] mac80211: fix a memory leak where sta_in ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1442</link>
      <description>Sasha Levin writes: (Summary) From: Ahmed Zaki &amp;lt;anzaki@gmail.com&amp;gt;&lt;br/&gt;
From: Ahmed Zaki &amp;lt;anzaki@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit 8f9dcc29566626f683843ccac6113a12208315ca ]
[ Upstream commit 8f9dcc29566626f683843ccac6113a12208315ca ]
The following is from a system that went OOM due to a memory leak:
The following is from a system that went OOM due to a memory leak:
wlan0: Allocated STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: Allocated STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: IBSS finish 74:83:c2:64:0b:87 (---from ieee80211_ibss_add_sta)
wlan0: Adding new IBSS station 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 2&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 3&lt;br/&gt;
wlan0: Inserted STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: IBSS finish 74:83:c2:64:0b:87 (---from ieee80211_ibss_work)
wlan0: Adding new IBSS station 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 2&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 3&lt;br/&gt;
.&lt;br/&gt;
.&lt;br/&gt;
wlan0: expiring inactive not authorized STA 74:83:c2:64:0b:87
wlan0: moving </description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 05/14] nl80211: reset regdom when reloading regdb</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1441</link>
      <description>Sasha Levin writes: (Summary) From: Finn Behrens &amp;lt;me@kloenk.dev&amp;gt;&lt;br/&gt;
From: Finn Behrens &amp;lt;me@kloenk.dev&amp;gt;&lt;br/&gt;
[ Upstream commit 1eda919126b420fee6b8d546f7f728fbbd4b8f11 ]
[ Upstream commit 1eda919126b420fee6b8d546f7f728fbbd4b8f11 ]
Reload the regdom when the regulatory db is reloaded. 27 +++++++++++++++++++++++++--
 2 files changed, 26 insertions(+), 2 deletions(-)

diff --git a/include/net/regulatory.h b/include/net/regulatory.h
index 3469750df0f44..16d61a0980e35 100644
--- a/include/net/regulatory.h
+++ b/include/net/regulatory.h
@@ -83,6 +83,7 @@ struct regulatory_request {
  enum nl80211_dfs_regions dfs_region;
+ if (!request) {
+  err = -ENOMEM;
+ }
+
+ request-&amp;gt;wiphy_idx = WIPHY_IDX_INVALID;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 06/14] iwlwifi: fix LED dependencies</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1440</link>
      <description>Sasha Levin writes: (Summary)  2 +-
 2 files changed, 3 insertions(+), 3 deletions(-)

diff --git a/drivers/net/wireless/intel/iwlegacy/Kconfig b/drivers/net/wireless/intel/iwlegacy/Kconfig
index 100f55858b133..490ef62605455 100644
--- a/drivers/net/wireless/intel/iwlegacy/Kconfig
+++ b/drivers/net/wireless/intel/iwlegacy/Kconfig
@@ -2,14 +2,13 @@
 config IWLEGACY
  tristate
  select FW_LOADER
- select NEW_LEDS
- select LEDS_CLASS
  select LEDS_TRIGGERS
  select MAC80211_LEDS
 
 config IWL4965
  tristate "Intel Wireless WiFi 4965AGN (iwl4965)"
  depends on PCI &amp;amp;&amp;amp;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 03/14] mac80211: set up the fwd_skb-&gt;dev for me ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1439</link>
      <description>Sasha Levin writes: (Summary) 
[   72.962020 ] CPU: 0 PID: 1193 Comm: kworker/u5:1 Tainted: P S 5.4.137 #0
[   72.969938 ] Hardware name: MT7622_MT7531 RFB (DT)
[   72.974659 ] Workqueue: napi_workq napi_workfn&lt;br/&gt;
[   72.979025 ] pstate: 60000005 (nZCv daif -PAN -UAO)
[   72.983822 ] pc : __skb_flow_dissect+0x284/0x1298
[   72.988444 ] lr : __skb_flow_dissect+0x54/0x1298
[   72.992977 ] sp : ffffffc010c738c0
[   72.996293 ] x29: ffffffc010c738c0 x28: 0000000000000000
[   73.001615 ] x27: 000000000000ffc2 x26: ffffff800c2eb818
[   73.006937 ] x25: ffffffc010a987c8 x24: 00000000000000ce
[   73.012259 ] x23: ffffffc010c73a28 x22: ffffffc010a99c60
[   73.017581 ] x21: 000000000000ffc2 x20: ffffff80094da800
[   73.022903 ] x19: 0000000000000000 x18: 0000000000000014
[   73.028226 ] x17: 00000000084d16af x16: 00000000d1fc0bab
[   73.033548 ] x15: 00000000715f6034 x14: 000000009dbdd301
[   73.038870 ] x13: 00000000ea4dcbc3 x12: 0000000000000040
[   73.044192 ] x11: 000000000eb00ff0 x10: 0000000000000000
[   73.049513 ] x9 : 000000000eb000</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 02/14] ARM: rockchip: Use memcpy_toio instead o ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1438</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
This fixes a potential kernel panic on memcpy when FORTIFY_SOURCE
is enabled. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/arch/arm/mach-rockchip/platsmp.c b/arch/arm/mach-rockchip/platsmp.c
index 649e0a54784cd..8ee6e4b309b37 100644
--- a/arch/arm/mach-rockchip/platsmp.c
+++ b/arch/arm/mach-rockchip/platsmp.c
@@ -189,7 +189,7 @@ static int __init rockchip_smp_prepare_sram(struct device_node *node)
  rockchip_boot_fn = __pa_symbol(secondary_startup);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.4 01/14] ARM: dts: ls1021a-tsn: update RGMII dela ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1437</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit e691f9282a89e24a8e87cdb91a181c6283ee5124 ]
[ Upstream commit e691f9282a89e24a8e87cdb91a181c6283ee5124 ]
In the new behavior, the sja1105 driver expects there to be explicit
RGMII delays present on the fixed-link ports, otherwise it will complain
that it falls back to legacy behavior, which is to apply RGMII delays
incorrectly derived from the phy-mode string.&lt;br/&gt;
incorrectly derived from the phy-mode string.&lt;br/&gt;
In this case, the legacy behavior of the driver is to not apply delays
in any direction (mostly because the SJA1105T can't do that, so this
board uses PCB traces).
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 19/19] Revert "block: reduce kblockd_mod_delay ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1436</link>
      <description>Sasha Levin writes: (Summary) From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
[ Upstream commit 87959fa16cfbcf76245c11559db1940069621274 ]
[ Upstream commit 87959fa16cfbcf76245c11559db1940069621274 ]
This reverts commit cb2ac2912a9ca7d3d26291c511939a41361d2d83. 2 --
 1 file changed, 2 deletions(-)

diff --git a/block/blk-core.c b/block/blk-core.c
index 4f4e286198660..26664f2a139eb 100644
--- a/block/blk-core.c
+++ b/block/blk-core.c
@@ -1657,8 +1657,6 @@ EXPORT_SYMBOL(kblockd_schedule_work);
 int kblockd_mod_delayed_work_on(int cpu, struct delayed_work *dwork,
     unsigned long delay)
 {
- if (!delay)
-  return queue_work_on(cpu, kblockd_workqueue, &amp;amp;dwork-&amp;gt;work);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 18/19] bpf: Make 32-&gt;64 bounds propagation sli ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1435</link>
      <description>Sasha Levin writes: (Summary) From: Daniel Borkmann &amp;lt;daniel@iogearbox.net&amp;gt;&lt;br/&gt;
From: Daniel Borkmann &amp;lt;daniel@iogearbox.net&amp;gt;&lt;br/&gt;
[ Upstream commit e572ff80f05c33cd0cb4860f864f5c9c044280b6 ]
[ Upstream commit e572ff80f05c33cd0cb4860f864f5c9c044280b6 ]
Make the bounds propagation in __reg_assign_32_into_64() slightly more
robust and readable by aligning it similarly as we did back in the
__reg_combine_64_into_32() counterpart.
- /* Attempt to pull 32-bit signed bounds into 64-bit bounds
-  * but must be positive otherwise set to worse case bounds
-  * and refine later from tnum.
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 17/19] virtio_net: fix rx_drops stat for small ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1434</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 053c9e18c6f9cf82242ef35ac21cae1842725714 ]
[ Upstream commit 053c9e18c6f9cf82242ef35ac21cae1842725714 ]
We found the stat of rx drops for small pkts does not increment when
build_skb fail, it's not coherent with other mode's rx drops stat. 9 +++------
 1 file changed, 3 insertions(+), 6 deletions(-)

diff --git a/drivers/net/virtio_net.c b/drivers/net/virtio_net.c
index cbe47eed7cc3c..3ba289b695f04 100644
--- a/drivers/net/virtio_net.c
+++ b/drivers/net/virtio_net.c
@@ -697,7 +697,7 @@ static struct sk_buff *receive_small(struct net_device *dev,
   pr_debug("%s: rx error: len %u exceeds max size %d\n",
     dev-&amp;gt;name, len, GOOD_PACKET_LEN);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 10/19] mac80211: Fix the size used for buildin ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1433</link>
      <description>Sasha Levin writes: (Summary) From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
Instead of using the hard-coded value of '100' use the correct
scan IEs length as calculated during HW registration to mac80211. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/net/mac80211/util.c b/net/mac80211/util.c
index fbf56a203c0e8..5dfa26b533802 100644
--- a/net/mac80211/util.c
+++ b/net/mac80211/util.c
@@ -2040,7 +2040,7 @@ struct sk_buff *ieee80211_build_probe_req(struct ieee80211_sub_if_data *sdata,
   chandef.chan = chan;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 13/19] drm/amd/display: Reset DMCUB before HW init</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1432</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 791255ca9fbe38042cfd55df5deb116dc11fef18 ]
[ Upstream commit 791255ca9fbe38042cfd55df5deb116dc11fef18 ]
[Why]&lt;br/&gt;
If the firmware wasn't reset by PSP or HW and is currently running
then the firmware will hang or perform underfined behavior when we
modify its firmware state underneath it.&lt;br/&gt;
modify its firmware state underneath it.&lt;br/&gt;
[How]&lt;br/&gt;
Reset DMCUB before setting up cache windows and performing HW init.
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 11/19] mac80211: update channel context before ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1431</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 4dde3c3627b52ca515a34f6f4de3898224aa1dd3 ]
[ Upstream commit 4dde3c3627b52ca515a34f6f4de3898224aa1dd3 ]
Currently channel context is updated only after station got an update about
new assoc state, this results in station using the old channel context. 15 +++++++++------
 1 file changed, 9 insertions(+), 6 deletions(-)

diff --git a/net/mac80211/sta_info.c b/net/mac80211/sta_info.c
index 461c03737da8d..3ccd1a311ff9c 100644
--- a/net/mac80211/sta_info.c
+++ b/net/mac80211/sta_info.c
@@ -668,6 +668,15 @@ static int sta_info_insert_finish(struct sta_info *sta) __acquires(RCU)
 
  list_add_tail_rcu(&amp;amp;sta-&amp;gt;list, &amp;amp;local-&amp;gt;sta_list);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 12/19] mac80211: do drv_reconfig_complete() be ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1430</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 13dee10b30c058ee2c58c5da00339cc0d4201aa6 ]
[ Upstream commit 13dee10b30c058ee2c58c5da00339cc0d4201aa6 ]
When we reconfigure, the driver might do some things to complete
the reconfiguration. 14 +++++++-------
 1 file changed, 7 insertions(+), 7 deletions(-)

diff --git a/net/mac80211/util.c b/net/mac80211/util.c
index 5dfa26b533802..dcebb0eb8a4be 100644
--- a/net/mac80211/util.c
+++ b/net/mac80211/util.c
@@ -2624,6 +2624,13 @@ int ieee80211_reconfig(struct ieee80211_local *local)
   mutex_unlock(&amp;amp;local-&amp;gt;sta_mtx);
 
- /*
-  * If this is for hw restart things are still running.
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 08/19] net: bcmgenet: Fix NULL vs IS_ERR() che ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1429</link>
      <description>Sasha Levin writes: (Summary) From: Miaoqian Lin &amp;lt;linmq006@gmail.com&amp;gt;&lt;br/&gt;
From: Miaoqian Lin &amp;lt;linmq006@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit ab8eb798ddabddb2944401bf31ead9671cb97d95 ]
[ Upstream commit ab8eb798ddabddb2944401bf31ead9671cb97d95 ]
The phy_attach() function does not return NULL. 4 ++--
 1 file changed, 2 insertions(+), 2 deletions(-)

diff --git a/drivers/net/ethernet/broadcom/genet/bcmmii.c b/drivers/net/ethernet/broadcom/genet/bcmmii.c
index f9e91304d2327..2b67e335bc54e 100644
--- a/drivers/net/ethernet/broadcom/genet/bcmmii.c
+++ b/drivers/net/ethernet/broadcom/genet/bcmmii.c
@@ -557,9 +557,9 @@ static int bcmgenet_mii_pd_init(struct bcmgenet_priv *priv)
    * Internal or external PHY with MDIO access
    */
   phydev = phy_attach(priv-&amp;gt;dev, phy_name, pd-&amp;gt;phy_interface);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 07/19] phonet: refcount leak in pep_sock_accep</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1428</link>
      <description>Sasha Levin writes: (Summary) From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
sock_hold(sk) is invoked in pep_sock_accept(), but __sock_put(sk) is not
invoked in subsequent failure branches(pep_accept_conn() != 0). 1 +
 1 file changed, 1 insertion(+)

diff --git a/net/phonet/pep.c b/net/phonet/pep.c
index a1525916885ae..b4f90afb0638b 100644
--- a/net/phonet/pep.c
+++ b/net/phonet/pep.c
@@ -868,6 +868,7 @@ static struct sock *pep_sock_accept(struct sock *sk, int flags, int *errp,
 
  err = pep_accept_conn(newsk, skb);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 05/19] nl80211: reset regdom when reloading regdb</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1427</link>
      <description>Sasha Levin writes: (Summary) From: Finn Behrens &amp;lt;me@kloenk.dev&amp;gt;&lt;br/&gt;
From: Finn Behrens &amp;lt;me@kloenk.dev&amp;gt;&lt;br/&gt;
[ Upstream commit 1eda919126b420fee6b8d546f7f728fbbd4b8f11 ]
[ Upstream commit 1eda919126b420fee6b8d546f7f728fbbd4b8f11 ]
Reload the regdom when the regulatory db is reloaded. 27 +++++++++++++++++++++++++--
 2 files changed, 26 insertions(+), 2 deletions(-)

diff --git a/include/net/regulatory.h b/include/net/regulatory.h
index 47f06f6f5a67c..0cf9335431e07 100644
--- a/include/net/regulatory.h
+++ b/include/net/regulatory.h
@@ -83,6 +83,7 @@ struct regulatory_request {
  enum nl80211_dfs_regions dfs_region;
+ if (!request) {
+  err = -ENOMEM;
+ }
+
+ request-&amp;gt;wiphy_idx = WIPHY_IDX_INVALID;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 02/19] ARM: rockchip: Use memcpy_toio instead  ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1426</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
This fixes a potential kernel panic on memcpy when FORTIFY_SOURCE
is enabled. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/arch/arm/mach-rockchip/platsmp.c b/arch/arm/mach-rockchip/platsmp.c
index d60856898d97a..5ec58d004b7de 100644
--- a/arch/arm/mach-rockchip/platsmp.c
+++ b/arch/arm/mach-rockchip/platsmp.c
@@ -189,7 +189,7 @@ static int __init rockchip_smp_prepare_sram(struct device_node *node)
  rockchip_boot_fn = __pa_symbol(secondary_startup);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 06/19] iwlwifi: fix LED dependencies</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1425</link>
      <description>Sasha Levin writes: (Summary)  2 +-
 2 files changed, 3 insertions(+), 3 deletions(-)

diff --git a/drivers/net/wireless/intel/iwlegacy/Kconfig b/drivers/net/wireless/intel/iwlegacy/Kconfig
index 24fe3f63c3215..7eacc8e58ee14 100644
--- a/drivers/net/wireless/intel/iwlegacy/Kconfig
+++ b/drivers/net/wireless/intel/iwlegacy/Kconfig
@@ -2,14 +2,13 @@
 config IWLEGACY
  tristate
  select FW_LOADER
- select NEW_LEDS
- select LEDS_CLASS
  select LEDS_TRIGGERS
  select MAC80211_LEDS
 
 config IWL4965
  tristate "Intel Wireless WiFi 4965AGN (iwl4965)"
  depends on PCI &amp;amp;&amp;amp;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 09/19] scsi: pm8001: Fix phys_to_virt() usage  ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1424</link>
      <description>Sasha Levin writes: (Summary) From: John Garry &amp;lt;john.garry@huawei.com&amp;gt;&lt;br/&gt;
From: John Garry &amp;lt;john.garry@huawei.com&amp;gt;&lt;br/&gt;
[ Upstream commit 2fe24343922e0428fb68674a4fae099171141bc7 ]
[ Upstream commit 2fe24343922e0428fb68674a4fae099171141bc7 ]
The driver supports a "direct" mode of operation, where the SMP req frame
is directly copied into the command payload (and vice-versa for the SMP
resp).&lt;br/&gt;
resp).&lt;br/&gt;
To get at the SMP req frame data in the scatterlist the driver uses
phys_to_virt() on the DMA mapped memory dma_addr_t .
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 04/19] mac80211: fix a memory leak where sta_i ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1423</link>
      <description>Sasha Levin writes: (Summary) From: Ahmed Zaki &amp;lt;anzaki@gmail.com&amp;gt;&lt;br/&gt;
From: Ahmed Zaki &amp;lt;anzaki@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit 8f9dcc29566626f683843ccac6113a12208315ca ]
[ Upstream commit 8f9dcc29566626f683843ccac6113a12208315ca ]
The following is from a system that went OOM due to a memory leak:
The following is from a system that went OOM due to a memory leak:
wlan0: Allocated STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: Allocated STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: IBSS finish 74:83:c2:64:0b:87 (---from ieee80211_ibss_add_sta)
wlan0: Adding new IBSS station 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 2&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 3&lt;br/&gt;
wlan0: Inserted STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: IBSS finish 74:83:c2:64:0b:87 (---from ieee80211_ibss_work)
wlan0: Adding new IBSS station 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 2&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 3&lt;br/&gt;
.&lt;br/&gt;
.&lt;br/&gt;
wlan0: expiring inactive not authorized STA 74:83:c2:64:0b:87
wlan0: moving </description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 29/29] Revert "block: reduce kblockd_mod_delay ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1422</link>
      <description>Sasha Levin writes: (Summary) From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
[ Upstream commit 87959fa16cfbcf76245c11559db1940069621274 ]
[ Upstream commit 87959fa16cfbcf76245c11559db1940069621274 ]
This reverts commit cb2ac2912a9ca7d3d26291c511939a41361d2d83. 2 --
 1 file changed, 2 deletions(-)

diff --git a/block/blk-core.c b/block/blk-core.c
index a728434fcff87..c2d912d0c976c 100644
--- a/block/blk-core.c
+++ b/block/blk-core.c
@@ -1625,8 +1625,6 @@ EXPORT_SYMBOL(kblockd_schedule_work);
 int kblockd_mod_delayed_work_on(int cpu, struct delayed_work *dwork,
     unsigned long delay)
 {
- if (!delay)
-  return queue_work_on(cpu, kblockd_workqueue, &amp;amp;dwork-&amp;gt;work);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 03/19] mac80211: set up the fwd_skb-&gt;dev for m ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1421</link>
      <description>Sasha Levin writes: (Summary) 
[   72.962020 ] CPU: 0 PID: 1193 Comm: kworker/u5:1 Tainted: P S 5.4.137 #0
[   72.969938 ] Hardware name: MT7622_MT7531 RFB (DT)
[   72.974659 ] Workqueue: napi_workq napi_workfn&lt;br/&gt;
[   72.979025 ] pstate: 60000005 (nZCv daif -PAN -UAO)
[   72.983822 ] pc : __skb_flow_dissect+0x284/0x1298
[   72.988444 ] lr : __skb_flow_dissect+0x54/0x1298
[   72.992977 ] sp : ffffffc010c738c0
[   72.996293 ] x29: ffffffc010c738c0 x28: 0000000000000000
[   73.001615 ] x27: 000000000000ffc2 x26: ffffff800c2eb818
[   73.006937 ] x25: ffffffc010a987c8 x24: 00000000000000ce
[   73.012259 ] x23: ffffffc010c73a28 x22: ffffffc010a99c60
[   73.017581 ] x21: 000000000000ffc2 x20: ffffff80094da800
[   73.022903 ] x19: 0000000000000000 x18: 0000000000000014
[   73.028226 ] x17: 00000000084d16af x16: 00000000d1fc0bab
[   73.033548 ] x15: 00000000715f6034 x14: 000000009dbdd301
[   73.038870 ] x13: 00000000ea4dcbc3 x12: 0000000000000040
[   73.044192 ] x11: 000000000eb00ff0 x10: 0000000000000000
[   73.049513 ] x9 : 000000000eb000</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 28/29] riscv: dts: sifive unmatched: Link the  ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1420</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit f6f7fbb89bf8dc9132fde55cfe67483138eea880 ]
[ Upstream commit f6f7fbb89bf8dc9132fde55cfe67483138eea880 ]
Fixes the following probe warning:&lt;br/&gt;
  lm90 0-004c: Looking up vcc-supply from device tree
  lm90 0-004c: Looking up vcc-supply property in node /soc/i2c@10030000/temperature-sensor@4c failed
  lm90 0-004c: supply vcc not found, using dummy regulator

Signed-off-by: Vincent Pelletier &amp;lt;plr.vincent@gmail.com&amp;gt; 1 +
 1 file changed, 1 insertion(+)

diff --git a/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts b/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts
index 09d9342282339..0c4a508869059 100644
--- a/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts
+++ b/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts
@@ -59,6 +59,7 @@ &amp;amp;i2c0 {
  temperature-sensor@4c {
   compatible = "ti,tmp451";
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.10 01/19] ARM: dts: ls1021a-tsn: update RGMII del ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1419</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit e691f9282a89e24a8e87cdb91a181c6283ee5124 ]
[ Upstream commit e691f9282a89e24a8e87cdb91a181c6283ee5124 ]
In the new behavior, the sja1105 driver expects there to be explicit
RGMII delays present on the fixed-link ports, otherwise it will complain
that it falls back to legacy behavior, which is to apply RGMII delays
incorrectly derived from the phy-mode string.&lt;br/&gt;
incorrectly derived from the phy-mode string.&lt;br/&gt;
In this case, the legacy behavior of the driver is to not apply delays
in any direction (mostly because the SJA1105T can't do that, so this
board uses PCB traces).
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 27/29] riscv: dts: sifive unmatched: Fix regul ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1418</link>
      <description>Sasha Levin writes: (Summary)  84 ++++++-------------
 1 file changed, 24 insertions(+), 60 deletions(-)

diff --git a/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts b/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts
index dd110ba00e871..09d9342282339 100644
--- a/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts
+++ b/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts
@@ -93,47 +93,31 @@ wdt {
   };
 
   regulators {
-   vdd_bcore1: bcore1 {
-    regulator-min-microvolt = &amp;lt;900000&amp;gt;;
-
-   vdd_bcore2: bcore2 {
-    regulator-min-microvolt = &amp;lt;900000&amp;gt;;
-
-   vdd_bio: bio {
+   vdd_bmem_bio: bmem-bio-merged {
     regulator-min-microvolt = &amp;lt;1200000&amp;gt;;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 25/29] riscv: dts: sifive unmatched: Expose th ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1417</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 8120393b74b31bbaf293f59896de6b0d50febc48 ]
[ Upstream commit 8120393b74b31bbaf293f59896de6b0d50febc48 ]
Mark it as read-only as it is factory-programmed with identifying
information, and no executable nor configuration:&lt;br/&gt;
- eth MAC address&lt;br/&gt;
- board model (PCB version, BoM version)&lt;br/&gt;
- board serial number&lt;br/&gt;
Accidental modification would cause misidentification which could brick
the board, so marking read-only seem like both a safe and non-constraining
choice.&lt;br/&gt;
choice.&lt;br/&gt;
Signed-off-by: Vincent Pelletier &amp;lt;plr.vincent@gmail.com&amp;gt;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 26/29] riscv: dts: sifive unmatched: Expose th ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1416</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit cd29cc8ad2540a4f9a0a3e174394d39e648ef941 ]
[ Upstream commit cd29cc8ad2540a4f9a0a3e174394d39e648ef941 ]
These sub-functions are available in the chip revision on this board, so
expose them.&lt;br/&gt;
expose them.&lt;br/&gt;
Signed-off-by: Vincent Pelletier &amp;lt;plr.vincent@gmail.com&amp;gt; 12 ++++++++++++
 1 file changed, 12 insertions(+)

diff --git a/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts b/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts
index 7ea1c8da5fb2e..dd110ba00e871 100644
--- a/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts
+++ b/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts
@@ -80,6 +80,18 @@ pmic@58 {
   interrupts = &amp;lt;1 IRQ_TYPE_LEVEL_LOW&amp;gt;;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 24/29] riscv: dts: sifive unmatched: Name gpio ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1415</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit ea81b91e4e256b0bb75d47ad3a5c230b2171a005 ]
[ Upstream commit ea81b91e4e256b0bb75d47ad3a5c230b2171a005 ]
Follow the pin descriptions given in the version 3 of the board schematics. 4 ++++
 1 file changed, 4 insertions(+)

diff --git a/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts b/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts
index 2e4ea84f27e77..a788b99638af8 100644
--- a/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts
+++ b/arch/riscv/boot/dts/sifive/hifive-unmatched-a00.dts
@@ -250,4 +250,8 @@ &amp;amp;pwm1 {
 
 &amp;amp;gpio {
  status = "okay";
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 22/29] virtio_net: fix rx_drops stat for small ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1414</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 053c9e18c6f9cf82242ef35ac21cae1842725714 ]
[ Upstream commit 053c9e18c6f9cf82242ef35ac21cae1842725714 ]
We found the stat of rx drops for small pkts does not increment when
build_skb fail, it's not coherent with other mode's rx drops stat. 9 +++------
 1 file changed, 3 insertions(+), 6 deletions(-)

diff --git a/drivers/net/virtio_net.c b/drivers/net/virtio_net.c
index 4ad25a8b0870c..1231b48f7183e 100644
--- a/drivers/net/virtio_net.c
+++ b/drivers/net/virtio_net.c
@@ -730,7 +730,7 @@ static struct sk_buff *receive_small(struct net_device *dev,
   pr_debug("%s: rx error: len %u exceeds max size %d\n",
     dev-&amp;gt;name, len, GOOD_PACKET_LEN);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 23/29] bpf: Make 32-&gt;64 bounds propagation sli ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1413</link>
      <description>Sasha Levin writes: (Summary) From: Daniel Borkmann &amp;lt;daniel@iogearbox.net&amp;gt;&lt;br/&gt;
From: Daniel Borkmann &amp;lt;daniel@iogearbox.net&amp;gt;&lt;br/&gt;
[ Upstream commit e572ff80f05c33cd0cb4860f864f5c9c044280b6 ]
[ Upstream commit e572ff80f05c33cd0cb4860f864f5c9c044280b6 ]
Make the bounds propagation in __reg_assign_32_into_64() slightly more
robust and readable by aligning it similarly as we did back in the
__reg_combine_64_into_32() counterpart.
- /* Attempt to pull 32-bit signed bounds into 64-bit bounds
-  * but must be positive otherwise set to worse case bounds
-  * and refine later from tnum.
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 21/29] net: usb: lan78xx: add Allied Telesis A ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1412</link>
      <description>Sasha Levin writes: (Summary)  6 ++++++
 1 file changed, 6 insertions(+)

diff --git a/drivers/net/usb/lan78xx.c b/drivers/net/usb/lan78xx.c
index a0401a9dade5b..3e1a83a22fdd6 100644
--- a/drivers/net/usb/lan78xx.c
+++ b/drivers/net/usb/lan78xx.c
@@ -76,6 +76,8 @@
 #define LAN7801_USB_PRODUCT_ID  (0x7801)
 #define LAN78XX_EEPROM_MAGIC  (0x78A5)
 #define LAN78XX_OTP_MAGIC  (0x78F3)
+#define AT29M2AF_USB_VENDOR_ID  (0x07C9)
+#define AT29M2AF_USB_PRODUCT_ID (0x0012)
 
 #define MII_READ   1
 #define MII_WRITE   0
@@ -4734,6 +4736,10 @@ static const struct usb_device_id products[] = {
  /* LAN7801 USB Gigabit Ethernet Device */
  USB_DEVICE(LAN78XX_USB_VENDOR_ID, LAN7801_USB_PRODUCT_ID),
  },
+ {
+ /* ATM2-AF USB Gigabit Ethernet Device */
+ USB_DEVICE(AT29M2AF_USB_VENDOR_ID, AT29M2AF_USB_PRODUCT_ID),
+ },
  {},
 };
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 20/29] block: reduce kblockd_mod_delayed_work_ ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1411</link>
      <description>Sasha Levin writes: (Summary) From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
From: Jens Axboe &amp;lt;axboe@kernel.dk&amp;gt;&lt;br/&gt;
[ Upstream commit cb2ac2912a9ca7d3d26291c511939a41361d2d83 ]
[ Upstream commit cb2ac2912a9ca7d3d26291c511939a41361d2d83 ]
Dexuan reports that he's seeing spikes of very heavy CPU utilization when
running 24 disks and using the 'none' scheduler. This happens off the
sched restart path, because SCSI requires the queue to be restarted async,
and hence we're hammering on mod_delayed_work_on() to ensure that the work
item gets run appropriately.&lt;br/&gt;
item gets run appropriately.&lt;br/&gt;
Avoid hammering on the timer and just use queue_work_on() if no delay
has been specified.&lt;br/&gt;
has been specified.&lt;br/&gt;
Reported-and-tested-by: Dexuan Cui &amp;lt;decui@microsoft.com&amp;gt;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 19/29] drm/amdgpu: correct the wrong cached st ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1410</link>
      <description>Sasha Levin writes: (Summary) From: Evan Quan &amp;lt;evan.quan@amd.com&amp;gt;&lt;br/&gt;
From: Evan Quan &amp;lt;evan.quan@amd.com&amp;gt;&lt;br/&gt;
[ Upstream commit 17c65d6fca844ee72a651944d8ce721e9040bf70 ]
[ Upstream commit 17c65d6fca844ee72a651944d8ce721e9040bf70 ]
Pair the operations did in GMC -&amp;gt;hw_init and -&amp;gt;hw_fini.
  }
 
+ /*
+  * Pair the operations did in gmc_v9_0_hw_init and thus maintain
+  * a correct cached state for GMC. AMD_PG_SUPPORT_MMHUB)
+  amdgpu_dpm_set_powergating_by_smu(adev,
+        AMD_IP_BLOCK_TYPE_GMC,
+        enable);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 18/29] drm/amd/display: Reset DMCUB before HW init</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1409</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 791255ca9fbe38042cfd55df5deb116dc11fef18 ]
[ Upstream commit 791255ca9fbe38042cfd55df5deb116dc11fef18 ]
[Why]&lt;br/&gt;
If the firmware wasn't reset by PSP or HW and is currently running
then the firmware will hang or perform underfined behavior when we
modify its firmware state underneath it.&lt;br/&gt;
modify its firmware state underneath it.&lt;br/&gt;
[How]&lt;br/&gt;
Reset DMCUB before setting up cache windows and performing HW init.
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 14/29] scsi: pm8001: Fix phys_to_virt() usage  ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1408</link>
      <description>Sasha Levin writes: (Summary) From: John Garry &amp;lt;john.garry@huawei.com&amp;gt;&lt;br/&gt;
From: John Garry &amp;lt;john.garry@huawei.com&amp;gt;&lt;br/&gt;
[ Upstream commit 2fe24343922e0428fb68674a4fae099171141bc7 ]
[ Upstream commit 2fe24343922e0428fb68674a4fae099171141bc7 ]
The driver supports a "direct" mode of operation, where the SMP req frame
is directly copied into the command payload (and vice-versa for the SMP
resp).&lt;br/&gt;
resp).&lt;br/&gt;
To get at the SMP req frame data in the scatterlist the driver uses
phys_to_virt() on the DMA mapped memory dma_addr_t .
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 17/29] mac80211: do drv_reconfig_complete() be ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1407</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 13dee10b30c058ee2c58c5da00339cc0d4201aa6 ]
[ Upstream commit 13dee10b30c058ee2c58c5da00339cc0d4201aa6 ]
When we reconfigure, the driver might do some things to complete
the reconfiguration. 14 +++++++-------
 1 file changed, 7 insertions(+), 7 deletions(-)

diff --git a/net/mac80211/util.c b/net/mac80211/util.c
index 39961a4f55d12..a94223710d48d 100644
--- a/net/mac80211/util.c
+++ b/net/mac80211/util.c
@@ -2644,6 +2644,13 @@ int ieee80211_reconfig(struct ieee80211_local *local)
   mutex_unlock(&amp;amp;local-&amp;gt;sta_mtx);
 
- /*
-  * If this is for hw restart things are still running.
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 15/29] mac80211: Fix the size used for buildin ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1406</link>
      <description>Sasha Levin writes: (Summary) From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
From: Ilan Peer &amp;lt;ilan.peer@intel.com&amp;gt;&lt;br/&gt;
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
[ Upstream commit f22d981386d12d1513bd2720fb4387b469124d4b ]
Instead of using the hard-coded value of '100' use the correct
scan IEs length as calculated during HW registration to mac80211. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/net/mac80211/util.c b/net/mac80211/util.c
index 03ea4f929b997..39961a4f55d12 100644
--- a/net/mac80211/util.c
+++ b/net/mac80211/util.c
@@ -2061,7 +2061,7 @@ struct sk_buff *ieee80211_build_probe_req(struct ieee80211_sub_if_data *sdata,
   chandef.chan = chan;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 16/29] mac80211: update channel context before ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1405</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 4dde3c3627b52ca515a34f6f4de3898224aa1dd3 ]
[ Upstream commit 4dde3c3627b52ca515a34f6f4de3898224aa1dd3 ]
Currently channel context is updated only after station got an update about
new assoc state, this results in station using the old channel context. 15 +++++++++------
 1 file changed, 9 insertions(+), 6 deletions(-)

diff --git a/net/mac80211/sta_info.c b/net/mac80211/sta_info.c
index 6eeef7a61927b..9e0bb402674c9 100644
--- a/net/mac80211/sta_info.c
+++ b/net/mac80211/sta_info.c
@@ -664,6 +664,15 @@ static int sta_info_insert_finish(struct sta_info *sta) __acquires(RCU)
 
  list_add_tail_rcu(&amp;amp;sta-&amp;gt;list, &amp;amp;local-&amp;gt;sta_list);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 13/29] net: ethernet: ti: add missing of_node_ ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1404</link>
      <description>Sasha Levin writes: (Summary) From: Wang Qing &amp;lt;wangqing@vivo.com&amp;gt;&lt;br/&gt;
From: Wang Qing &amp;lt;wangqing@vivo.com&amp;gt;&lt;br/&gt;
[ Upstream commit be565ec71d1d59438bed0c7ed0a252a327e0b0ef ]
[ Upstream commit be565ec71d1d59438bed0c7ed0a252a327e0b0ef ]
Fix following coccicheck warning:&lt;br/&gt;
WARNING: Function "for_each_child_of_node"
should have of_node_put() before return.&lt;br/&gt;
should have of_node_put() before return.&lt;br/&gt;
Early exits from for_each_child_of_node should decrement the
node reference counter.&lt;br/&gt;
node reference counter.&lt;br/&gt;
Signed-off-by: Wang Qing &amp;lt;wangqing@vivo.com&amp;gt;&lt;br/&gt;
Signed-off-by: David S.
-   if (ret)
-    return dev_err_probe(dev, ret,
+   if (ret) {
+    ret = dev_err_probe(dev, ret,
            "failed to register fixed-link phy %pOF\n",
            port_np);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 08/29] iwlwifi: fix LED dependencies</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1403</link>
      <description>Sasha Levin writes: (Summary)  2 +-
 2 files changed, 3 insertions(+), 3 deletions(-)

diff --git a/drivers/net/wireless/intel/iwlegacy/Kconfig b/drivers/net/wireless/intel/iwlegacy/Kconfig
index 24fe3f63c3215..7eacc8e58ee14 100644
--- a/drivers/net/wireless/intel/iwlegacy/Kconfig
+++ b/drivers/net/wireless/intel/iwlegacy/Kconfig
@@ -2,14 +2,13 @@
 config IWLEGACY
  tristate
  select FW_LOADER
- select NEW_LEDS
- select LEDS_CLASS
  select LEDS_TRIGGERS
  select MAC80211_LEDS
 
 config IWL4965
  tristate "Intel Wireless WiFi 4965AGN (iwl4965)"
  depends on PCI &amp;amp;&amp;amp;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 12/29] net: bcmgenet: Fix NULL vs IS_ERR() che ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1402</link>
      <description>Sasha Levin writes: (Summary) From: Miaoqian Lin &amp;lt;linmq006@gmail.com&amp;gt;&lt;br/&gt;
From: Miaoqian Lin &amp;lt;linmq006@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit ab8eb798ddabddb2944401bf31ead9671cb97d95 ]
[ Upstream commit ab8eb798ddabddb2944401bf31ead9671cb97d95 ]
The phy_attach() function does not return NULL. 4 ++--
 1 file changed, 2 insertions(+), 2 deletions(-)

diff --git a/drivers/net/ethernet/broadcom/genet/bcmmii.c b/drivers/net/ethernet/broadcom/genet/bcmmii.c
index 89d16c587bb7d..50ea9401f123d 100644
--- a/drivers/net/ethernet/broadcom/genet/bcmmii.c
+++ b/drivers/net/ethernet/broadcom/genet/bcmmii.c
@@ -557,9 +557,9 @@ static int bcmgenet_mii_pd_init(struct bcmgenet_priv *priv)
    * Internal or external PHY with MDIO access
    */
   phydev = phy_attach(priv-&amp;gt;dev, phy_name, pd-&amp;gt;phy_interface);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 11/29] dmaengine: ti: k3-udma: Fix smatch warnings</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1401</link>
      <description>Sasha Levin writes: (Summary) From: Vignesh Raghavendra &amp;lt;vigneshr@ti.com&amp;gt;&lt;br/&gt;
From: Vignesh Raghavendra &amp;lt;vigneshr@ti.com&amp;gt;&lt;br/&gt;
[ Upstream commit 80936d68665be88dc3bf60884a71f2694eb6b1f1 ]
[ Upstream commit 80936d68665be88dc3bf60884a71f2694eb6b1f1 ]
Smatch reports below warnings [1] wrt dereferencing rm_res when it can
potentially be ERR_PTR(). j++, i++) {
-  if (rm_res-&amp;gt;desc[j].num) {
-   irq_res.desc[i].start = rm_res-&amp;gt;desc[j].start +
-     ud-&amp;gt;soc_data-&amp;gt;oes.udma_rchan; j++, i++) {
+   if (rm_res-&amp;gt;desc[j].num) {
+    irq_res.desc[i].start = rm_res-&amp;gt;desc[j].start +
+      ud-&amp;gt;soc_data-&amp;gt;oes.udma_rchan;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 10/29] net: usb: qmi_wwan: add Telit 0x1070 co ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1400</link>
      <description>Sasha Levin writes: (Summary) From: Daniele Palmas &amp;lt;dnlplm@gmail.com&amp;gt;&lt;br/&gt;
From: Daniele Palmas &amp;lt;dnlplm@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit 94f2a444f28a649926c410eb9a38afb13a83ebe0 ]
[ Upstream commit 94f2a444f28a649926c410eb9a38afb13a83ebe0 ]
Add the following Telit FN990 composition:&lt;br/&gt;
Add the following Telit FN990 composition:&lt;br/&gt;
0x1070: tty, adb, rmnet, tty, tty, tty, tty&lt;br/&gt;
0x1070: tty, adb, rmnet, tty, tty, tty, tty&lt;br/&gt;
Signed-off-by: Daniele Palmas &amp;lt;dnlplm@gmail.com&amp;gt; 1 +
 1 file changed, 1 insertion(+)

diff --git a/drivers/net/usb/qmi_wwan.c b/drivers/net/usb/qmi_wwan.c
index 33ada2c59952e..b62489f567e48 100644
--- a/drivers/net/usb/qmi_wwan.c
+++ b/drivers/net/usb/qmi_wwan.c
@@ -1355,6 +1355,7 @@ static const struct usb_device_id products[] = {
  {QMI_QUIRK_SET_DTR(0x1bc7, 0x1040, 2)}, /* Telit LE922A */
  {QMI_QUIRK_SET_DTR(0x1bc7, 0x1050, 2)}, /* Telit FN980 */
  {QMI_QUIRK_SET_DTR(0x1bc7, 0x1060, 2)}, /* Telit LN920 */
+ {QMI_QUIRK_SET_DTR(0x1bc7, 0x1070, 2)}, /* Telit FN990 */
  {QMI_FIXED_INTF(0x1bc7,</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 09/29] phonet: refcount leak in pep_sock_accep</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1399</link>
      <description>Sasha Levin writes: (Summary) From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
From: Hangyu Hua &amp;lt;hbh25y@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
[ Upstream commit bcd0f93353326954817a4f9fa55ec57fb38acbb0 ]
sock_hold(sk) is invoked in pep_sock_accept(), but __sock_put(sk) is not
invoked in subsequent failure branches(pep_accept_conn() != 0). 1 +
 1 file changed, 1 insertion(+)

diff --git a/net/phonet/pep.c b/net/phonet/pep.c
index a1525916885ae..b4f90afb0638b 100644
--- a/net/phonet/pep.c
+++ b/net/phonet/pep.c
@@ -868,6 +868,7 @@ static struct sock *pep_sock_accept(struct sock *sk, int flags, int *errp,
 
  err = pep_accept_conn(newsk, skb);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 06/29] mac80211: fix a memory leak where sta_i ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1398</link>
      <description>Sasha Levin writes: (Summary) From: Ahmed Zaki &amp;lt;anzaki@gmail.com&amp;gt;&lt;br/&gt;
From: Ahmed Zaki &amp;lt;anzaki@gmail.com&amp;gt;&lt;br/&gt;
[ Upstream commit 8f9dcc29566626f683843ccac6113a12208315ca ]
[ Upstream commit 8f9dcc29566626f683843ccac6113a12208315ca ]
The following is from a system that went OOM due to a memory leak:
The following is from a system that went OOM due to a memory leak:
wlan0: Allocated STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: Allocated STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: IBSS finish 74:83:c2:64:0b:87 (---from ieee80211_ibss_add_sta)
wlan0: Adding new IBSS station 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 2&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 3&lt;br/&gt;
wlan0: Inserted STA 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: IBSS finish 74:83:c2:64:0b:87 (---from ieee80211_ibss_work)
wlan0: Adding new IBSS station 74:83:c2:64:0b:87&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 2&lt;br/&gt;
wlan0: moving STA 74:83:c2:64:0b:87 to state 3&lt;br/&gt;
.&lt;br/&gt;
.&lt;br/&gt;
wlan0: expiring inactive not authorized STA 74:83:c2:64:0b:87
wlan0: moving </description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 07/29] nl80211: reset regdom when reloading regdb</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1397</link>
      <description>Sasha Levin writes: (Summary) From: Finn Behrens &amp;lt;me@kloenk.dev&amp;gt;&lt;br/&gt;
From: Finn Behrens &amp;lt;me@kloenk.dev&amp;gt;&lt;br/&gt;
[ Upstream commit 1eda919126b420fee6b8d546f7f728fbbd4b8f11 ]
[ Upstream commit 1eda919126b420fee6b8d546f7f728fbbd4b8f11 ]
Reload the regdom when the regulatory db is reloaded. 27 +++++++++++++++++++++++++--
 2 files changed, 26 insertions(+), 2 deletions(-)

diff --git a/include/net/regulatory.h b/include/net/regulatory.h
index 47f06f6f5a67c..0cf9335431e07 100644
--- a/include/net/regulatory.h
+++ b/include/net/regulatory.h
@@ -83,6 +83,7 @@ struct regulatory_request {
  enum nl80211_dfs_regions dfs_region;
+ if (!request) {
+  err = -ENOMEM;
+ }
+
+ request-&amp;gt;wiphy_idx = WIPHY_IDX_INVALID;
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 05/29] mac80211: set up the fwd_skb-&gt;dev for m ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1396</link>
      <description>Sasha Levin writes: (Summary) 
[   72.962020 ] CPU: 0 PID: 1193 Comm: kworker/u5:1 Tainted: P S 5.4.137 #0
[   72.969938 ] Hardware name: MT7622_MT7531 RFB (DT)
[   72.974659 ] Workqueue: napi_workq napi_workfn&lt;br/&gt;
[   72.979025 ] pstate: 60000005 (nZCv daif -PAN -UAO)
[   72.983822 ] pc : __skb_flow_dissect+0x284/0x1298
[   72.988444 ] lr : __skb_flow_dissect+0x54/0x1298
[   72.992977 ] sp : ffffffc010c738c0
[   72.996293 ] x29: ffffffc010c738c0 x28: 0000000000000000
[   73.001615 ] x27: 000000000000ffc2 x26: ffffff800c2eb818
[   73.006937 ] x25: ffffffc010a987c8 x24: 00000000000000ce
[   73.012259 ] x23: ffffffc010c73a28 x22: ffffffc010a99c60
[   73.017581 ] x21: 000000000000ffc2 x20: ffffff80094da800
[   73.022903 ] x19: 0000000000000000 x18: 0000000000000014
[   73.028226 ] x17: 00000000084d16af x16: 00000000d1fc0bab
[   73.033548 ] x15: 00000000715f6034 x14: 000000009dbdd301
[   73.038870 ] x13: 00000000ea4dcbc3 x12: 0000000000000040
[   73.044192 ] x11: 000000000eb00ff0 x10: 0000000000000000
[   73.049513 ] x9 : 000000000eb000</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 04/29] mac80211: fix TCP performance on mesh i ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1395</link>
      <description>Sasha Levin writes: (Summary) From: Maxime Bizon &amp;lt;mbizon@freebox.fr&amp;gt;&lt;br/&gt;
From: Maxime Bizon &amp;lt;mbizon@freebox.fr&amp;gt;&lt;br/&gt;
[ Upstream commit 48c06708e63e71b4395e4159797366aa03be10ff ]
[ Upstream commit 48c06708e63e71b4395e4159797366aa03be10ff ]
sta is NULL for mesh point (resolved later), so sk pacing parameters
were not applied.&lt;br/&gt;
were not applied.&lt;br/&gt;
Signed-off-by: Maxime Bizon &amp;lt;mbizon@freebox.fr&amp;gt; 4 ++--
 1 file changed, 2 insertions(+), 2 deletions(-)

diff --git a/net/mac80211/tx.c b/net/mac80211/tx.c
index 0527bf41a32c7..0613b3ab523a5 100644
--- a/net/mac80211/tx.c
+++ b/net/mac80211/tx.c
@@ -4190,11 +4190,11 @@ void __ieee80211_subif_start_xmit(struct sk_buff *skb,
 
  ieee80211_aggr_check(sdata, sta, skb);
</description>
    </item>
    <item>
      <title>[PATCH AUTOSEL 5.15 03/29] ARM: rockchip: Use memcpy_toio instead  ...</title>
      <author>Sasha Levin &lt;invalid@email.com&gt;</author>
      <link>http://lkml.org/lkml/2021/12/20/1394</link>
      <description>Sasha Levin writes: (Summary) 
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
[ Upstream commit 423e85e97aaf69e5198bbec6811e3825c8b5019a ]
This fixes a potential kernel panic on memcpy when FORTIFY_SOURCE
is enabled. 2 +-
 1 file changed, 1 insertion(+), 1 deletion(-)

diff --git a/arch/arm/mach-rockchip/platsmp.c b/arch/arm/mach-rockchip/platsmp.c
index d60856898d97a..5ec58d004b7de 100644
--- a/arch/arm/mach-rockchip/platsmp.c
+++ b/arch/arm/mach-rockchip/platsmp.c
@@ -189,7 +189,7 @@ static int __init rockchip_smp_prepare_sram(struct device_node *node)
  rockchip_boot_fn = __pa_symbol(secondary_startup);
</description>
    </item>
  </channel>
</rss> ]
  pull_request:
    branches: [ master ]

  # Allows you to run this workflow manually from the Actions tab
  workflow_dispatch:

# A workflow run is made up of one or more jobs that can run sequentially or in parallel
jobs:
  # This workflow contains a single job called "build"
  build:
    # The type of runner that the job will run on
    runs-on: ubuntu-latest

    # Steps represent a sequence of tasks that will be executed as part of the job
    steps:
      # Checks-out your repository under $GITHUB_WORKSPACE, so your job can access it
      - uses: actions/checkout@v2

      # Runs a single command using the runners shell
      - name: Run a one-line script
        run: echo Hello, world!

      # Runs a set of commands using the runners shell
      - name: Run a multi-line script
        run: |
          echo Add other actions to build,
          echo test, and deploy your project.
      # Routing with EdgeJS https://github.com/Nate158s

The `{{ PACKAGE_NAME }}/core` package provides a JavaScript API for controlling routing and caching from your code base rather than a CDN web portal. Using this _{{ EDGEJS_LABEL }}_ approach allows this vital routing logic to be properly tested, reviewed, and version controlled, just like the rest of your application code.

Using the Router, you can:

- Proxy requests to upstream sites
- Send redirects from the network edge
- Render responses on the server using Next.js, Nuxt.js, Angular, or any other framework that supports server side rendering.
- Alter request and response headers
- Send synthetic responses
- Configure multiple destinations for split testing

## Configuration

To define routes for {{ PRODUCT_NAME }}, create a `routes.js` file in the root of your project. You can override the default path to the router by setting the `routes` key in `{{ CONFIG_FILE }}`.

The `routes.js` file should export an instance of `{{ PACKAGE_NAME }}/core/router/Router`:

```js
// routes.js
const { Router } = require('{{ PACKAGE_NAME }}/core/router')

module.exports = new Router()
```

## Declare Routes

Declare routes using the method corresponding to the HTTP method you want to match.

```js
// routes.js
const { Router } = require('{{ PACKAGE_NAME }}/core/router')

module.exports = new Router().get('/some-path', ({ cache, proxy }) => {
  // handle the request here
})
```

All HTTP methods are available:

- get
- put
- post
- patch
- delete
- head

To match all methods, use `match`:

```js
// routes.js
const { Router } = require('{{ PACKAGE_NAME }}/core/router')

module.exports = new Router().match('/some-path', ({ cache, proxy }) => {
  // handle the request here
})
```

## Route Execution

When {{ PRODUCT_NAME }} receives a request, it executes **each route that matches the request** in the order in which they are declared until one sends a response. The following methods return a response:

- [appShell](/docs/api/core/classes/_router_responsewriter_.responsewriter.html#appshell)
- [compute](/docs/api/core/classes/_router_responsewriter_.responsewriter.html#compute)
- [proxy](/docs/api/core/classes/_router_responsewriter_.responsewriter.html#proxy)
- [redirect](/docs/api/core/classes/_router_responsewriter_.responsewriter.html#redirect)
- [send](/docs/api/core/classes/_router_responsewriter_.responsewriter.html#send)
- [serveStatic](/docs/api/core/classes/_router_responsewriter_.responsewriter.html#servestatic)
- [serviceWorker](/docs/api/core/classes/_router_responsewriter_.responsewriter.html#serviceworker)
- [stream](/docs/api/core/classes/_router_responsewriter_.responsewriter.html#stream)
- [use](/docs/api/core/classes/_router_responsewriter_.responsewriter.html#compute)

Multiple routes can therefore be executed for a given request. A common pattern is to add caching with one route and render the response with a later one using middleware. In the following example we cache then render a response with Next.js:

```js
const { Router } = require('{{ PACKAGE_NAME }}/core/router')
const { nextRoutes } = require('{{ PACKAGE_NAME }}/next')

// In this example a request to /products/1 will be cached by the first route, then served by the `nextRoutes` middleware
new Router()
  .get('/products/:id', ({ cache }) => {
    cache({
      edge: { maxAgeSeconds: 60 * 60, staleWhileRevalidateSeconds: 60 * 60 },
    })
  })
  .use(nextRoutes)
```

### Alter Requests and Responses

{{ PRODUCT_NAME }} offers APIs to manipulate request and response headers and cookies. The APIs are:

| Operation     | Request               | Upstream Response              | Response sent to Browser |
| ------------- | --------------------- | ------------------------------ | ------------------------ |
| Set header    | `setRequestHeader`    | `setUpstreamResponseHeader`    | `setResponseHeader`      |
| Add cookie    | `*`                   | `addUpstreamResponseCookie`    | `addResponseCookie`      |
| Update header | `updateRequestHeader` | `updateUpstreamResponseHeader` | `updateResponseHeader`   |
| Update cookie | `*`                   | `updateUpstreamResponseCookie` | `updateResponseCookie`   |
| Remove header | `removeRequestHeader` | `removeUpstreamResponseHeader` | `removeResponseHeader`   |
| Remove cookie | `*`                   | `removeUpstreamResponseCookie` | `removeResponseCookie`   |

`*` Adding, updating, or removing a request cookie can be achieved with `updateRequestHeader` applied to `cookie` header.

You can find detailed descriptions of these APIs in the `{{ PACKAGE_NAME }}/core` [documentation](/docs/api/core/classes/_router_responsewriter_.responsewriter.html).

#### Embedded Values

You can inject values from the request or response into headers or cookies as template literals using the `${value}` format. For example: `setResponseHeader('original-request-path', '${path}')` would add an `original-request-path` response header whose value is the request path.

| Value           | Embedded value         | Description                                                                                                                   |
| --------------- | ---------------------- | ----------------------------------------------------------------------------------------------------------------------------- |
| HTTP method     | `${method}`            | The value of the HTTP method used for the request (e.g. `GET`)                                                                |
| URL             | `${url}`               | The complete URL path including any query strings (e.g. `/search?query=docs`). Protocol, hostname, and port are not included. |
| Path            | `${path}`              | The URL path excluding any query strings (e.g. `/search`)                                                                     |
| Query string    | `${query:<name>}`      | The value of the `<name>` query string or empty if not available.                                                             |
| Request header  | `${req:<name>}`        | The value of the `<name>` request header or empty if not available.                                                           |
| Request cookie  | `${req:cookie:<name>}` | The value of the `<name>` cookie in `cookie` request header or empty if not available.                                        |
| Response header | `${res:<name>}`        | The value of the `<name>` response header or empty if not available.                                                          |

## Route Pattern Syntax

The syntax for route paths is provided by [path-to-regexp](https://github.com/pillarjs/path-to-regexp#path-to-regexp), which is the same library used by [Express](https://expressjs.com/).

### Named Parameters

Named parameters are defined by prefixing a colon to the parameter name (`:foo`).

```js
new Router().get('/:foo/:bar', res => {
  /* ... */
})
```

**Please note:** Parameter names must use "word characters" (`[A-Za-z0-9_]`).

#### Custom Matching Parameters

Parameters can have a custom regexp, which overrides the default match (`[^/]+`). For example, you can match digits or names in a path:

```js
new Router().get('/icon-:foo(\\d+).png', res => {
  /* ... */
})
```

**Tip:** Backslashes need to be escaped with another backslash in JavaScript strings.

#### Custom Prefix and Suffix

Parameters can be wrapped in `{}` to create custom prefixes or suffixes for your segment:

```js
new Router().get('/:attr1?{-:attr2}?{-:attr3}?', res => {
  /* ... */
})
```

### Unnamed Parameters

It is possible to write an unnamed parameter that only consists of a regexp. It works the same the named parameter, except it will be numerically indexed:

```js
new Router().get('/:foo/(.*)', res => {
  /* ... */
})
```

### Modifiers

Modifiers must be placed after the parameter (e.g. `/:foo?`, `/(test)?`, `/:foo(test)?`, or `{-:foo(test)}?`).

#### Optional

Parameters can be suffixed with a question mark (`?`) to make the parameter optional.

```js
new Router().get('/:foo/:bar?', res => {
  /* ... */
})
```

**Tip:** The prefix is also optional, escape the prefix `\/` to make it required.

#### Zero or More

Parameters can be suffixed with an asterisk (`*`) to denote zero or more parameter matches.

```js
new Router().get('/:foo*', res => {
  /* res.params.foo will be an array */
})
```

The captured parameter value will be provided as an array.

#### One or More

Parameters can be suffixed with a plus sign (`+`) to denote one or more parameter matches.

```js
new Router().get('/:foo+', res => {
  /* res.params.foo will be an array */
})
```

The captured parameter value will be provided as an array.

## Matching Method, Query Parameters, Cookies, and Headers

Match can either take a URL path, or an object which allows you to match based on method, query parameters, cookies, or request headers:

```js
router.match(
  {
    path: '/some-path', // value is route-pattern syntax
    method: /GET|POST/i, // value is a regular expression
    cookies: { currency: /^(usd)$/i }, // keys are cookie names, values are regular expressions
    headers: { 'x-moov-device': /^desktop$/i }, // keys are header names, values are regular expressions
    query: { page: /^(1|2|3)$/ }, // keys are query parameter names, values are regular expressions
  },
  () => {},
)
```

## Body Matching for POST requests

You can also match HTTP `POST` requests based on their request body content as in the following example:

```js
router.match(
  {
    body: { parse: 'json', criteria: { operationName: 'GetProducts' } }, // the body content will parsed as JSON and the parsed JSON matched against the presence of the criteria properties (in this case a GraphQL operation named 'GetProducts')
  },
  () => {},
)
```

Currently the only body content supported is JSON. Body content is parsed as JSON and is matched against the presence of the fields specified in the `criteria` field. The [_POST Body Matching Criteria_](#section_post_body_matching_criteria) section below contains examples of using the `criteria` field.

Body matching can be combined with other match parameters such as headers and cookies. For example,

```js
router.match(
  {
    // Only matches GetProducts operations to the /graphql endpoint
    // for logged in users
    path: '/graphql',
    cookies: { loginStatus: /^(loggedIn)$/i }, // loggedin users
    body: { parse: 'json', criteria: { operationName: 'GetProducts' } },
  },
  () => {},
)
```

### Caching & POST Body Matching

When body matching is combined with `cache` in a route, **the HTTP request body will automatically be used as the cache key.** For example, the code below will cache GraphQL `GetProducts` queries using the entire request body as the cache key:

```js
router.match(
  {
    body: { parse: 'json', criteria: { operationName: 'GetProducts' } },
  },
  ({ cache }) => {
    cache({
      edge: {
        maxAgeSeconds: 60 * 60,
        staleWhileRevalidateSeconds: 60 * 60 * 24, // this way stale items can still be prefetched
      },
    })
  },
)
```

You can still add additional parameters to the cache key using the normal {{ EDGEJS_LABEL }} `key` property. For example, the code below will cache GraphQL `GetProducts` queries separately for each user based on their userID cookie _and_ the HTTP body of the request.

```js
router.match(
  {
    body: { parse: 'json', criteria: { operationName: 'GetProducts' } },
  },
  ({ cache }) => {
    cache({
      edge: {
        maxAgeSeconds: 60 * 60,
        staleWhileRevalidateSeconds: 60 * 60 * 24, // this way stale items can still be prefetched
      },
      key: new CustomCacheKey().addCookie('userID'), // Split cache by userID
    })
  },
)
```

### POST Body Matching Criteria

The `criteria` property can be a string or regular expression.

For example, the router below,

```js
router.match(
  {
    body: { parse: 'json', criteria: { foo: 'bar' } },
  },
  () => {},
)
```

would match an HTTP POST request body containing:

```js
{
  "foo": "bar",
  "bar": "foo"
}
```

### Regular Expression Criteria

Regular expressions can also be used as `criteria`. For example,

```js
router.match(
  {
    body: { parse: 'json', criteria: { operationName: /^Get/ } },
  },
  () => {},
)
```

would match an HTTP POST body containing:

```js
{
  "operationName": "GetShops",
  "query": "...",
  "variables": {}
}
```

### Nested JSON Criteria

You can also use a nested object to match a field at a specific location in the JSON. For example,

```js
router.match(
  {
    body: {
      parse: 'json',
      criteria: {
        operation: {
          name: 'GetShops',
        },
      },
    },
  },
  () => {},
)
```

would match an HTTP POST body containing:

```js
{
  "operation": {
    "name": "GetShops",
    "query": "..."
  }
}
```

## GraphQL Queries

The {{ EDGEJS_LABEL }} router provides a `graphqlOperation` method for matching GraphQL.

```js
router.graphqlOperation('GetProducts', res => {
  /* Handle the POST for the GetProducts query specifically */
})
```

By default, the `graphqlOperation` assumes your GraphQL endpoint is at `/graphql`. You can alter this behavior by using the `path` property as shown below:

```js
router.graphqlOperation({ path: '/api/graphql', name: 'GetProducts' }, res => {
  /* Handle the POST for the GetProducts query specifically */
})
```

Note that when the `graphqlOperation` function is used, the HTTP request body will automatically be included in the cache key.

The `graphqlOperation` function is provided to simplify matching of common GraphQL scenarios. For complex GraphQL matching (such as authenticated data), you can use the generic [_Body Matching for POST requests_](#section_body_matching_for_post_requests) feature.

See the guide on [Implementing GraphQL Routing](/guides/graphql) in your project.

## Request Handling 

The second argument to routes is a function that receives a `ResponseWriter` and uses it to send a response. Using `ResponseWriter` you can:

- Proxy a backend configured in `{{ CONFIG_FILE }}`
- Serve a static file
- Send a redirect
- Send a synthetic response
- Cache the response at edge and in the browser
- Manipulate request and response headers

[See the API Docs for Response Writer](/docs/__version__/api/core/classes/_router_responsewriter_.responsewriter.html)

## Full Example

This example shows typical usage of `{{ PACKAGE_NAME }}/core`, including serving a service worker, next.js routes (vanity and conventional routes), and falling back to a legacy backend.

```js
// routes.js

const { Router } = require('{{ PACKAGE_NAME }}/core/router')

module.exports = new Router()
  .get('/service-worker.js', ({ serviceWorker }) => {
    // serve the service worker built by webpack
    serviceWorker('dist/service-worker.js')
  })
  .get('/p/:productId', ({ cache }) => {
    // cache products for one hour at edge and using the service worker
    cache({
      edge: {
        maxAgeSeconds: 60 * 60,
        staleWhileRevalidateSeconds: 60 * 60,
      },
      browser: {
        maxAgeSeconds: 0,
        serviceWorkerSeconds: 60 * 60,
      },
    })
    proxy('origin')
  })
  .fallback(({ proxy }) => {
    // serve all unmatched URLs from the origin backend configured in {{ CONFIG_FILE }}
    proxy('origin')
  })
```

## Errors Handling

You can use the router's `catch` method to return specific content when the request results in an error status (For example, a 500). Using `catch`, you can also alter the `statusCode` and `response` on the edge before issuing a response to the user.

```js
router.catch(number | Regexp, (routeHandler: Function))
```

### Examples

To issue a custom error page when the origin returns a 500:

```js
// routes.js

const { Router } = require('{{ PACKAGE_NAME }}/core/router')

module.exports = new Router()
  // Example route
  .get('/failing-route', ({ proxy }) => {
    proxy('broken-origin')
  })
  // So let's assume that backend "broken-origin" returns 500, so instead
  // of rendering the broken-origin response we can alter that by specifing .catch
  .catch(500, ({ serveStatic }) => {
    serveStatic('static/broken-origin-500-page.html', {
      statusCode: 502,
    })
  })
```

The `.catch` method allows the edge router to render a response based on the result preceeding routes. So in the example above whenever we receive a 500 we respond with `broken-origin-500-page.html` from the application's `static` directory and change the status code to 502.

- Your catch callback is provided a [ResponseWriter](/docs/api/core/classes/_router_responsewriter_.responsewriter.html) instance. You can use any ResponseWriter method except `proxy` inside `.catch`.
- We highly recommend keeping `catch` routes simple. Serve responses using `serveStatic` instead of `send` to minimize the size of the edge bundle.

## Environment Edge Redirects

In addition to sending redirects at the edge within the router configuration, this can also be configured at the environment level within the Layer0 Developer Console.

Under _&lt;Your Environment&gt; &#8594; Configuration_, click _Edit_ to draft a new configuration. Scroll down to the _Redirects_ section:
![redirects](/images/environments/redirects.png)

Click _Add A Redirect_ to configure the path or host you wish to redirect to:
![add redirect](/images/environments/add_redirects.png)

**Note:** you will need to activate and redeploy your site for this change to take effect.
