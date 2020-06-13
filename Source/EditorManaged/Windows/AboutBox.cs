//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
using bs;

namespace bs.Editor
{
    /** @addtogroup Windows
     *  @{
     */

    /// <summary>
    /// Displays information about the engine, its creators and licenses.
    /// </summary>
    public class AboutBox : ModalWindow
    {
        private GUITextBox emailLabel;

        /// <summary>
        /// Opens the about box.
        /// </summary>
        [MenuItem("Help/About", 5000)]
        public static void Open()
        {
            new AboutBox();
        }

        /// <summary>
        /// Constructs the about box.
        /// </summary>
        protected AboutBox()
            : base(true)
        {
            Title = "About Banshee";
            Width = 450;
            Height = 400;
        }

        private void OnInitialize()
        {
            GUILabel title = new GUILabel(new LocEdString("Banshee Editor " + EngineVersion.GetFullVersionString()), EditorStyles.TitleLabel);
            GUILabel subTitle = new GUILabel(new LocEdString("A modern open-source game development toolkit"), 
                EditorStyles.LabelCentered);
            GUILabel bsfVersion = new GUILabel(new LocEdString("Powered by bs::framework " + FrameworkVersion.GetFullVersionString()), EditorStyles.LabelCentered);
            GUILabel license = new GUILabel(new LocEdString(
                "This program is licensed under the GNU Lesser General Public License V3 or later"), EditorStyles.LabelCentered);
            GUILabel copyright = new GUILabel(new LocEdString("Copyright (C) 2014 - " + EngineVersion.GetCurrentReleaseYearNumber() + " Marko Pintera and contributors. All rights reserved."), 
                EditorStyles.LabelCentered);

            GUILabel editorContributorsLabel = new GUILabel(new LocEdString("Editor contributors"), EditorStyles.TitleLabel);
            GUILabel frameworkContributorsLabel = new GUILabel(new LocEdString("Framework contributors"), EditorStyles.TitleLabel);
            GUILabel engineLabel = new GUILabel(new LocEdString("Editor and engine"), EditorStyles.TitleLabel);
            GUILabel frameworkLabel = new GUILabel(new LocEdString("Framework"), EditorStyles.TitleLabel);


            GUILabel authorLabel = new GUILabel(new LocEdString("Banshee was created, and is being actively developed by Marko Pintera."));
            GUILabel emailTitle = new GUILabel(new LocEdString("E-mail"), GUIOption.FixedWidth(150));
            emailLabel = new GUITextBox();
            GUILabel linkedInTitle = new GUILabel(new LocEdString("LinkedIn"), GUIOption.FixedWidth(150));
            GUIButton linkedInBtn = new GUIButton(new LocEdString("Profile"));
            GUIButton patreonBtn = new GUIButton(new LocEdString("Support us on Patreon!"), GUIOption.FixedWidth(210));
            GUIButton paypalBtn = new GUIButton(new LocEdString("Donate via PayPal!"), GUIOption.FixedWidth(210));
            
            GUIScrollArea scrollArea = new GUIScrollArea();
            GUIToggleGroup foldoutGroup = new GUIToggleGroup(true);
            GUIToggle contactFoldout = new GUIToggle(new LocEdString("Main author"), foldoutGroup, EditorStyles.Foldout);
            GUIToggle thirdPartyFoldout = new GUIToggle(new LocEdString("Used third party libraries"), foldoutGroup, EditorStyles.Foldout);
            GUIToggle noticesFoldout = new GUIToggle(new LocEdString("Third party notices"), foldoutGroup, EditorStyles.Foldout);
            GUIToggle contributorsFoldout = new GUIToggle(new LocEdString("Contributors"), foldoutGroup, EditorStyles.Foldout);
            GUIToggle socialFoldout = new GUIToggle(new LocEdString("Social"), foldoutGroup, EditorStyles.Foldout);

            contactFoldout.AcceptsKeyFocus = false;
            thirdPartyFoldout.AcceptsKeyFocus = false;
            noticesFoldout.AcceptsKeyFocus = false;
            contributorsFoldout.AcceptsKeyFocus = false;
            socialFoldout.AcceptsKeyFocus = false;

            GUILabel freeTypeNotice = new GUILabel(new LocEdString(
                        "Portions of this software are copyright (C) 2019 The FreeType Project (www.freetype.org). " +
                        "All rights reserved."), EditorStyles.MultiLineLabelCentered, 
                        GUIOption.FlexibleHeight(), GUIOption.FixedWidth(380));

            GUILabel fbxSdkNotice = new GUILabel(new LocEdString(
                "This software contains Autodesk(R) FBX(R) code developed by Autodesk, Inc. Copyright 2019 Autodesk, Inc. " +
                "All rights, reserved. Such code is provided \"as is\" and Autodesk, Inc. disclaims any and all warranties, " +
                "whether express or implied, including without limitation the implied warranties of merchantability, " +
                "fitness for a particular purpose or non-infringement of third party rights. In no event shall Autodesk, " +
                "Inc. be liable for any direct, indirect, incidental, special, exemplary, or consequential damages " +
                "(including, but not limited to, procurement of substitute goods or services; loss of use, data, or " +
                "profits; or business interruption) however caused and on any theory of liability, whether in contract, " +
                "strict liability, or tort (including negligence or otherwise) arising in any way out of such code."),
                EditorStyles.MultiLineLabelCentered, GUIOption.FlexibleHeight(), GUIOption.FixedWidth(380));
            
            string[ , ] contributorListEditor = new string[ , ]
            {
                { "Danijel Ribic", "Logo, UI icons, 3D models & textures"},
                { "Marco Bellan", "Bugfixes, editor enhancements" },
                { "Robert Campbell", "Editor enhancements" },
                { "James Mitchell", "Build enhancements"},
                { "Patrick Recko", "Minor enhancements" }
            };

            string[] contributorListFramework = new string[]
            {
                "Marc Legendre",
                "Florian Will",
                "Marco Bellan",
                "Artur K. (@nemerle)",
                "Michael Jones",
                "Connor Fitzgerald",
                "Paolo Paoletto",
                "Patrick Recko",
                "Guillaume Meunier",
                "Andre Taulien",
                "James Mitchell",
                "and others..."
            };

            GUILayoutY mainLayout = GUI.AddLayoutY();
            mainLayout.AddSpace(10);
            mainLayout.AddElement(title);
            mainLayout.AddElement(subTitle);
            mainLayout.AddSpace(10);
            mainLayout.AddElement(bsfVersion);
            mainLayout.AddSpace(10);
            mainLayout.AddElement(license);
            mainLayout.AddSpace(5);
            mainLayout.AddElement(copyright);
            mainLayout.AddSpace(10);
            GUILayoutX donateButtonLayoutX = mainLayout.AddLayoutX();
            donateButtonLayoutX.AddSpace(10);
            donateButtonLayoutX.AddElement(patreonBtn);
            donateButtonLayoutX.AddSpace(10);
            donateButtonLayoutX.AddElement(paypalBtn);
            donateButtonLayoutX.AddSpace(10);
            mainLayout.AddSpace(10);

            mainLayout.AddElement(scrollArea);
            scrollArea.Layout.AddElement(contactFoldout);
            GUILayoutY contactLayout = scrollArea.Layout.AddLayoutY();
            contactLayout.AddSpace(15);
            GUILayout authorLayout = contactLayout.AddLayoutX();
            authorLayout.AddFlexibleSpace();
            authorLayout.AddElement(authorLabel);
            authorLayout.AddFlexibleSpace();
            contactLayout.AddSpace(15);
            GUILayout emailLayout = contactLayout.AddLayoutX();
            emailLayout.AddSpace(10);
            emailLayout.AddElement(emailTitle);
            emailLayout.AddElement(emailLabel);
            emailLayout.AddSpace(10);
            GUILayout linkedInLayout = contactLayout.AddLayoutX();
            linkedInLayout.AddSpace(10);
            linkedInLayout.AddElement(linkedInTitle);
            linkedInLayout.AddElement(linkedInBtn);
            linkedInLayout.AddSpace(10);

            scrollArea.Layout.AddSpace(5);
            scrollArea.Layout.AddElement(thirdPartyFoldout);
            GUILayoutY thirdPartyLayout = scrollArea.Layout.AddLayoutY();
            thirdPartyLayout.AddSpace(5);
            CreateNameURLBtnPair(thirdPartyLayout, "Autodesk FBX SDK",
                "https://www.autodesk.com/products/fbx/overview", "Website");
            CreateNameURLBtnPair(thirdPartyLayout, "FreeImage", "http://freeimage.sourceforge.net/", "Website");
            CreateNameURLBtnPair(thirdPartyLayout, "FreeType", "http://www.freetype.org/", "Website");
            CreateNameURLBtnPair(thirdPartyLayout, "Mono", "http://www.mono-project.com/", "Website");
            CreateNameURLBtnPair(thirdPartyLayout, "NVIDIA Texture Tools",
                "https://github.com/castano/nvidia-texture-tools", "Website");
            CreateNameURLBtnPair(thirdPartyLayout, "libFLAC", "https://xiph.org/flac/", "Website");
            CreateNameURLBtnPair(thirdPartyLayout, "libOgg", "https://www.xiph.org/ogg/", "Website");
            CreateNameURLBtnPair(thirdPartyLayout, "libVorbis", "http://www.vorbis.com/", "Website");
            CreateNameURLBtnPair(thirdPartyLayout, "OpenAL Soft", "http://kcat.strangesoft.net/openal.html", "Website");
            CreateNameURLBtnPair(thirdPartyLayout, "NVIDIA PhysX", "https://developer.nvidia.com/physx-sdk", "Website");

            scrollArea.Layout.AddSpace(5);
            scrollArea.Layout.AddElement(noticesFoldout);
            GUILayout noticesLayout = scrollArea.Layout.AddLayoutY();
            noticesLayout.AddSpace(5);
            noticesLayout.AddElement(freeTypeNotice);
            noticesLayout.AddSpace(10);
            noticesLayout.AddElement(fbxSdkNotice);

            scrollArea.Layout.AddSpace(5);
            scrollArea.Layout.AddElement(contributorsFoldout);
            GUILayoutY contributorsLayout = scrollArea.Layout.AddLayoutY();
            contributorsLayout.AddSpace(5);
            contributorsLayout.AddElement(editorContributorsLabel);
            contributorsLayout.AddSpace(5);
            for (int i = 0; i < contributorListEditor.Length / 2; i++)
            {
                CreateEditorContributorGUI(contributorsLayout, contributorListEditor[i, 0], contributorListEditor[i, 1]);
                contributorsLayout.AddSpace(5);
            }
            contributorsLayout.AddSpace(10);
            contributorsLayout.AddElement(frameworkContributorsLabel);
            contributorsLayout.AddSpace(5);
            for (int i = 0; i < contributorListFramework.Length; i++)
            {
                CreateFrameworkContributorGUI(contributorsLayout, contributorListFramework[i]);
                contributorsLayout.AddSpace(5);
            }

            scrollArea.Layout.AddSpace(5);
            scrollArea.Layout.AddElement(socialFoldout);
            GUILayoutY socialLayout = scrollArea.Layout.AddLayoutY();
            socialLayout.AddSpace(10);
            CreateNameURLBtnPair(socialLayout, "Discord", "https://discord.gg/8Xyf5gF", "Enter");
            socialLayout.AddSpace(10);
            socialLayout.AddElement(engineLabel);
            socialLayout.AddSpace(5);
            CreateNameURLBtnPair(socialLayout, "Website", "http://www.banshee3d.com", "Enter");
            CreateNameURLBtnPair(socialLayout, "Repository", "https://github.com/BearishSun/BansheeEngine", "Enter");
            CreateNameURLBtnPair(socialLayout, "Forums", "https://forum.banshee3d.com", "Enter");
            CreateNameURLBtnPair(socialLayout, "Twitter", "https://twitter.com/Banshee3D", "Enter");
            CreateNameURLBtnPair(socialLayout, "Facebook", "https://www.facebook.com/Banshee3D", "Enter");
            socialLayout.AddSpace(10);
            socialLayout.AddElement(frameworkLabel);
            socialLayout.AddSpace(5);
            CreateNameURLBtnPair(socialLayout, "Website", "http://www.bsframework.io", "Enter");
            CreateNameURLBtnPair(socialLayout, "Repository", "https://github.com/GameFoundry/bsf", "Enter");
            CreateNameURLBtnPair(socialLayout, "Forums", "https://discourse.bsframework.io/", "Enter");
            CreateNameURLBtnPair(socialLayout, "Twitter", "https://twitter.com/thebsframework", "Enter");
            CreateNameURLBtnPair(socialLayout, "Facebook", "https://www.facebook.com/bsframework", "Enter");
            mainLayout.AddFlexibleSpace();

            contactLayout.Active = false;
            contactFoldout.OnToggled += x => contactLayout.Active = x;

            thirdPartyLayout.Active = false;
            thirdPartyFoldout.OnToggled += x => thirdPartyLayout.Active = x;

            noticesLayout.Active = false;
            noticesFoldout.OnToggled += x => noticesLayout.Active = x;

            contributorsLayout.Active = false;
            contributorsFoldout.OnToggled += x => contributorsLayout.Active = x;

            socialLayout.Active = false;
            socialFoldout.OnToggled += x => socialLayout.Active = x;

            emailLabel.Text = "marko.pintera@gmail.com";
            linkedInBtn.OnClick += () => { System.Diagnostics.Process.Start("https://www.linkedin.com/in/markopintera"); };
            patreonBtn.OnClick += () => { System.Diagnostics.Process.Start("https://www.patreon.com/bsf"); };
            paypalBtn.OnClick += () => { System.Diagnostics.Process.Start("https://www.paypal.me/MarkoPintera/10"); };
        }

        private void CreateNameURLBtnPair(GUILayoutY layout, string name, string webURL, string btnText)
        {
            GUILabel label = new GUILabel(new LocEdString(name), GUIOption.FixedWidth(300));
            GUIButton linkBtn = new GUIButton(new LocEdString(btnText), GUIOption.FixedWidth(100));

            GUILayoutX horzLayout = layout.AddLayoutX();
            horzLayout.AddSpace(10);
            horzLayout.AddElement(label);
            horzLayout.AddSpace(10);
            horzLayout.AddElement(linkBtn);
            horzLayout.AddSpace(10);

            linkBtn.OnClick += () => { System.Diagnostics.Process.Start(webURL); };
        }

        private void CreateEditorContributorGUI(GUILayoutY layout, string name, string area)
        {
            GUILabel nameLabel = new GUILabel(new LocEdString(name), GUIOption.FixedWidth(150));
            GUILabel areaLabel = new GUILabel(new LocEdString(area), GUIOption.FixedWidth(220));

            GUILayoutX horzLayout = layout.AddLayoutX();
            horzLayout.AddSpace(10);
            horzLayout.AddElement(nameLabel);
            horzLayout.AddSpace(10);
            horzLayout.AddElement(areaLabel);
            horzLayout.AddSpace(10);
        }

        private void CreateFrameworkContributorGUI(GUILayoutY layout, string name)
        {
            GUILabel nameLabel = new GUILabel(new LocEdString(name), EditorStyles.LabelCentered);

            GUILayoutX horzLayout = layout.AddLayoutX();
            horzLayout.AddElement(nameLabel);
        }

        private void OnEditorUpdate()
        {
            emailLabel.Text = "marko.pintera@gmail.com";
        }
    }

    /** @} */
}
