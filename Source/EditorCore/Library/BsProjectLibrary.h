//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#pragma once

#include "BsEditorPrerequisites.h"
#include "Utility/BsModule.h"
#include "Threading/BsAsyncOp.h"
#include "Utility/BsUSPtr.h"

namespace bs
{
	/** @addtogroup Library
	 *  @{
	 */

	/**
	 * Project library is the primary location for interacting with all the resources in the current project. A complete
	 * hierarchy of resources is provided which can be interacted with by importing new ones, deleting them, moving,
	 * renaming and similar.
	 */
	class BS_ED_EXPORT ProjectLibrary : public Module<ProjectLibrary>
	{
	public:
		struct LibraryEntry;
		struct FileEntry;
		struct DirectoryEntry;

		/** Types of elements in the library, either a file or a folder. */
		enum class LibraryEntryType
		{
			File,
			Directory
		};

		/**	A generic library entry that may be a file or a folder depending on its type. */
		struct LibraryEntry
		{
			LibraryEntry();
			LibraryEntry(const Path& path, const String& name, DirectoryEntry* parent, LibraryEntryType type);

			LibraryEntryType type; /**< Specific type of this entry. */
			Path path; /**< Absolute path to the entry. */
			String elementName; /**< Name of the entry. */
			size_t elementNameHash = 0; /**< Hash of @p elementName, used for faster comparisons. */

			DirectoryEntry* parent = nullptr; /**< Folder this entry is located in. */
		};

		/**	A library entry representing a file. Each file can have one or multiple resources. */
		struct FileEntry : public LibraryEntry
		{
			FileEntry() = default;
			FileEntry(const Path& path, const String& name, DirectoryEntry* parent);

			SPtr<ProjectFileMeta> meta; /**< Meta file containing various information about the resource(s). */
			std::time_t lastUpdateTime = 0; /**< Timestamp of when we last imported the resource. */
		};

		/**	A library entry representing a folder that contains other entries. */
		struct DirectoryEntry : public LibraryEntry
		{
			DirectoryEntry() = default;
			DirectoryEntry(const Path& path, const String& name, DirectoryEntry* parent);

			Vector<USPtr<LibraryEntry>> mChildren; /**< Child files or folders. */
		};

	public:
		ProjectLibrary();
		~ProjectLibrary();

		/**
		 * Checks if any resources at the specified path have been modified, added or deleted, and updates the internal
		 * hierarchy accordingly. Automatically imports dirty resources.
		 *
		 * @param[in]	path	Absolute path of the file or folder to check. If a folder is provided all its children will
		 *						be checked recursively.
		 * @return				Returns the number of resources that were queued for import during this call.
		 */
		UINT32 checkForModifications(const Path& path);

		/**	Returns the root library entry that references the entire library hierarchy. */
		const USPtr<DirectoryEntry>& getRootEntry() const { return mRootEntry; }

		/**
		 * Attempts to a find a library entry at the specified path.
		 *
		 * @param[in]	path	Path to the entry, either absolute or relative to resources folder.
		 * @return				Found entry, or null if not found. Value returned by this method is transient, it may be
		 *						destroyed on any following ProjectLibrary call.
		 */
		USPtr<LibraryEntry> findEntry(const Path& path) const;

		/** 
		 * Checks whether the provided path points to a sub-resource. Sub-resource is any resource that is not the primary
		 * resource in the file.
		 */
		bool isSubresource(const Path& path) const;

		/**
		 * Attempts to a find a meta information for a resource at the specified path.
		 *
		 * @param[in]	path	Path to the entry, either absolute or relative to resources folder. If a sub-resource within
		 *						a file is needed, append the name of the subresource to the path 
		 *						(for example mymesh.fbx/my_animation).
		 * @return				Found meta information for the resource, or null if not found. 
		 */
		SPtr<ProjectResourceMeta> findResourceMeta(const Path& path) const;

		/**
		 * Searches the library for a pattern and returns all entries matching it.
		 *
		 * @param[in]	pattern	Pattern to search for. Use wildcard * to match any character(s).
		 * @return		A list of entries matching the pattern. Values returned by this method are transient, they may be
		 *				destroyed on any following ProjectLibrary call.
		 */
		Vector<USPtr<LibraryEntry>> search(const String& pattern);

		/**
		 * Searches the library for a pattern, but only among specific resource types.
		 *
		 * @param[in]	pattern	Pattern to search for. Use wildcard * to match any character(s).
		 * @param[in]	typeIds	RTTI type IDs of the resource types we're interested in searching.
		 * @return		A list of entries matching the pattern. Values returned by this method are transient, they may be
		 *				destroyed on any following ProjectLibrary call.
		 */
		Vector<USPtr<LibraryEntry>> search(const String& pattern, const Vector<UINT32>& typeIds);

		/**
		 * Returns resource path based on its UUID.
		 *
		 * @param[in]	uuid	UUID of the resource to look for.
		 * @return				Absolute path to the resource.
		 */
		Path uuidToPath(const UUID& uuid) const;

		/**
		 * Registers a new resource in the library.
		 *
		 * @param[in]	resource	Resource instance to add to the library. A copy of the resource will be saved at the
		 *							provided path.
		 * @param[in]	path		Path where where to store the resource. Absolute or relative to the resources folder.
		 */
		void createEntry(const HResource& resource, const Path& path);

		/**
		 * Creates a new folder in the library.
		 *
		 * @param[in]	path		Path where where to store the folder. Absolute or relative to the resources folder.
		 */
		void createFolderEntry(const Path& path);

		/**	Updates a resource that is already in the library. */
		void saveEntry(const HResource& resource);

		/**
		 * Moves a library entry from one path to another.
		 *
		 * @param[in]	oldPath		Source path of the entry, absolute or relative to resources folder.
		 * @param[in]	newPath		Destination path of the entry, absolute or relative to resources folder.
		 * @param[in]	overwrite	If an entry already exists at the destination path, should it be overwritten.
		 */
		void moveEntry(const Path& oldPath, const Path& newPath, bool overwrite = true);

		/**
		 * Copies a library entry from one path to another.
		 *
		 * @param[in]	oldPath		Source path of the entry, absolute or relative to resources folder.
		 * @param[in]	newPath		Destination path of the entry, absolute or relative to resources folder.
		 * @param[in]	overwrite	If an entry already exists at the destination path, should it be overwritten.
		 */
		void copyEntry(const Path& oldPath, const Path& newPath, bool overwrite = true);

		/**
		 * Deletes an entry from the library.
		 *
		 * @param[in]	path		Path of the entry, absolute or relative to resources folder.
		 */
		void deleteEntry(const Path& path);

		/**
		 * Triggers a reimport of a resource using the provided import options, if needed.
		 *
		 * @param[in]	path			Path to the resource to reimport, absolute or relative to resources folder.
		 * @param[in]	importOptions	Optional import options to use when importing the resource. Caller must ensure the
		 *								import options are of the correct type for the resource in question. If null is
		 *								provided default import options are used.
		 * @param[in]	forceReimport	Should the resource be reimported even if no changes are detected. This should be
		 *								true if import options changed since last import.
		 * @param[in]	synchronous		If true the import will happen synchronously on the calling thread. If false
		 *								the import operation will be queued for execution on a worker thread. You
		 *								then must call _finishQueuedImports() after the worker thread finishes to
		 *								actually finish the import.
		 */
		void reimport(const Path& path, const SPtr<ImportOptions>& importOptions = nullptr, bool forceReimport = false,
			bool synchronous = false);

		/**
		 * Checks how far along is the import for the specified file.
		 * 
		 * @param[in]	path		Path to the resource to check the progress for, absolute or relative to the resources 
		 *							folder.
		 * @return					Reports 1 if the file is fully imported. Reports 0 if the import has not started or the
		 *							file isn't even queued for import. Reports >= 0 if the file is in process of being
		 *							imported. Note that not all importers support fine grained progress reporting, in which
		 *							case the import progress will be reported as a binary 0 or 1.
		 */
		float getImportProgress(const Path& path) const;

		/** 
		 * Cancels any queued import tasks. Note that you must call _finishQueuedImports() for the import state to be
		 * updated. If the import task has already started you will need to wait until it finishes as there is no way to 
		 * stop running tasks. If the provided file entry isn't being imported, or has already finished imported, the 
		 * function does nothing. 
		 */
		void cancelImport();

		/**
		 * Determines if this resource will always be included in the build, regardless if it's being referenced or not.
		 *
		 * @param[in]	path	Path to the resource to modify, absolute or relative to resources folder.
		 * @param[in]	force	True if we want the resource to be included in the build, false otherwise.
		 */
		void setIncludeInBuild(const Path& path, bool force);

		/** 
		 * Assigns a non-specific user data object to the resource at the specified path. 
		 * 
		 * @param[in]	path		Path to the resource to modify, absolute or relative to resources folder.
		 * @param[in]	userData	User data to assign to the resource, which can later be retrieved from the resource's
		 *							meta-data as needed. 
		 */
		void setUserData(const Path& path, const SPtr<IReflectable>& userData);

		/**
		 * Finds all top-level resource entries that should be included in a build. Values returned by this method are
		 * transient, they may be destroyed on any following ProjectLibrary call.
		 */
		Vector<USPtr<FileEntry>> getResourcesForBuild() const;

		/**
		 * Loads a resource at the specified path, synchronously.
		 *
		 * @param[in]	path	Path of the resource, absolute or relative to resources folder. If a sub-resource within
		 *						a file is needed, append the name of the subresource to the path 
		 *						(for example mymesh.fbx/my_animation).
		 * @return				Loaded resource, or null handle if one is not found.
		 */
		HResource load(const Path& path);

		/** Returns the path to the project's resource folder where all the assets are stored. */
		const Path& getResourcesFolder() const { return mResourcesFolder; }

		/** Returns the number of resources currently queued for import. */
		UINT32 getInProgressImportCount() const { return (UINT32)mQueuedImports.size(); }

		/**
		 * Saves all the project library data so it may be restored later, at the default save location in the project
		 * folder. Project must be loaded when calling this.
		 */
		void saveLibrary();

		/**
		 * Loads previously saved project library data from the default save location in the project folder. Nothing is
		 * loaded if it doesn't exist.Project must be loaded when calling this.
		 */
		void loadLibrary();

		/**	Clears all library data. */
		void unloadLibrary();

		/** Triggered whenever an entry is removed from the library. Path provided is absolute. */
		Event<void(const Path&)> onEntryRemoved; 

		/** Triggered whenever an entry is added to the library. Path provided is absolute. */
		Event<void(const Path&)> onEntryAdded; 

		/** Triggered when a resource is being (re)imported. Path provided is absolute. */
		Event<void(const Path&)> onEntryImported; 

		/** @name Internal 
		 *  @{
		 */

		/**	Returns the resource manifest managed by the project library. */
		const SPtr<ResourceManifest>& _getManifest() const { return mResourceManifest; }

		/** 
		 * Iterates over any queued import operations, checks if they have finished and finalizes them. This should be
		 * called on a regular basis (e.g. every frame).
		 *
		 * @param[in]	wait	If true the method will block until all imports finish.
		 */
		void _finishQueuedImports(bool wait = false);

		/** @} */

		static const Path RESOURCES_DIR;
		static const Path INTERNAL_RESOURCES_DIR;
	private:
		/** Name/resource pair for a single imported resource. */
		struct QueuedImportResource
		{
			QueuedImportResource(String name, SPtr<Resource> resource, const UUID& uuid)
				:name(std::move(name)), resource(std::move(resource)), uuid(uuid)
			{ }

			QueuedImportResource(String name, const HResource& handle)
				:name(std::move(name)), resource(handle.getInternalPtr()), handle(handle), uuid(handle.getUUID())
			{ }

			String name;
			SPtr<Resource> resource;
			HResource handle;
			UUID uuid;
		};

		/** Information about an asynchronously queued import. */
		struct QueuedImport
		{
			Path filePath;
			SPtr<Task> importTask;
			SPtr<ImportOptions> importOptions;
			Vector<QueuedImportResource> resources;
			SPtr<QueuedImport> dependsOn;
			bool pruneMetas = false;
			bool canceled = false;
			bool native = false;
			std::time_t timestamp = 0;
		};

		/**
		 * Common code for adding a new resource entry to the library.
		 *
		 * @param[in]	parent			Parent of the new entry.
		 * @param[in]	filePath		Absolute path to the resource.
		 * @param[in]	importOptions	Optional import options to use when importing the resource. Caller must ensure the
		 *								import options are of the correct type for the resource in question. If null is 
		 *								provided default import options are used.
		 * @param[in]	forceReimport	Should the resource be reimported even if we detect no changes. This should be true
		 *								if import options changed since last import.
		 * @param[in]	synchronous		If true the import will happen synchronously on the calling thread. If false
		 *								the import operation will be queued for execution on a worker thread. You
		 *								then must call _finishQueuedImports() after the worker thread finishes to
		 *								actually finish the import.
		 * @return						Newly added resource entry.
		 */
		USPtr<FileEntry> addResourceInternal(DirectoryEntry* parent, const Path& filePath, 
			const SPtr<ImportOptions>& importOptions = nullptr, bool forceReimport = false, bool synchronous = false);

		/**
		 * Common code for adding a new folder entry to the library.
		 *
		 * @param[in]	parent	Parent of the new entry.
		 * @param[in]	dirPath	Absolute path to the directory.
		 * @return		Newly added directory entry.
		 */
		USPtr<DirectoryEntry> addDirectoryInternal(DirectoryEntry* parent, const Path& dirPath);

		/**
		 * Common code for deleting a resource from the library. This code only removes the library entry, not the actual
		 * resource file.
		 *
		 * @param[in]	resource	Entry to delete.
		 */
		void deleteResourceInternal(USPtr<FileEntry> resource);

		/**
		 * Common code for deleting a directory from the library. This code only removes the library entry, not the actual
		 * directory.
		 *
		 * @param[in]	directory	Entry to delete.
		 */
		void deleteDirectoryInternal(USPtr<DirectoryEntry> directory);

		/**
		 * Triggers a reimport of a resource using the provided import options, if needed. Doesn't import dependencies.
		 *
		 * @param[in]	fileEntry			File entry of the resource to reimport.
		 * @param[in]	importOptions		Optional import options to use when importing the resource. Caller must ensure 
		 *									the import options are of the correct type for the resource in question. If null
		 *									is provided default import options are used.
		 * @param[in]	forceReimport		Should the resource be reimported even if we detect no changes. This should be 
		 *									true if import options changed since last import.
		 * @param[in]	pruneResourceMetas	Determines should resource meta data for resources that no longer exist within
		 *									the file be deleted. Default behaviour is to keep these alive so that if their
		 *									resources are eventually restored, references to them will remain valid. If you
		 *									feel that you need to clear this data, set this to true but be aware that you
		 *									might need to re-apply those references.
		 * @param[in]	synchronous			If true the import will happen synchronously on the calling thread. If false
		 *									the import operation will be queued for execution on a worker thread. You
		 *									then must call _finishQueuedImports() after the worker thread finishes to
		 *									actually finish the import.
		 * @return							Returns true if the resource was queued for import (or imported, if 
		 *									synchronous), false otherwise.
		 */
		bool reimportResourceInternal(FileEntry* fileEntry, const SPtr<ImportOptions>& importOptions = nullptr, 
			bool forceReimport = false, bool pruneResourceMetas = false, bool synchronous = false);

		/**
		 * Creates a full hierarchy of directory entries up to the provided directory, if any are needed.
		 *
		 * @param[in]	fullPath			Absolute path to a directory we are creating the hierarchy to.
		 * @param[out]	newHierarchyRoot	First directory entry that already existed in our hierarchy.
		 * @param[out]	newHierarchyLeaf	Leaf entry corresponding to the exact entry at \p fullPath.
		 */
		void createInternalParentHierarchy(const Path& fullPath, DirectoryEntry** newHierarchyRoot, 
			DirectoryEntry** newHierarchyLeaf);

		/**	Checks has a file been modified since the last import. */
		bool isUpToDate(FileEntry* file) const;

		/**	Checks is the resource a native engine resource that doesn't require importing. */
		bool isNative(const Path& path) const;

		/**
		 * Returns a path to a .meta file based on the resource path.
		 *
		 * @param[in]	path	Absolute path to the resource.
		 * @return				Path to the .meta file.
		 */
		Path getMetaPath(const Path& path) const;
		
		/**	Checks does the path represent a .meta file. */
		bool isMeta(const Path& fullPath) const;

		/**
		 * Returns a set of resource paths that are dependent on the provided resource entry. (for example a shader file
		 * might be dependent on shader include file).
		 */
		Vector<Path> getImportDependencies(const FileEntry* entry);

		/**	Registers any import dependencies for the specified resource. */
		void addDependencies(const FileEntry* entry);

		/**	Removes any import dependencies for the specified resource. */
		void removeDependencies(const FileEntry* entry);

		/**	Finds dependants resource for the specified resource entry and reimports them. */
		void reimportDependants(const Path& entryPath);

		/**	Makes all library entry paths relative to the current resources folder. */
		void makeEntriesRelative();

		/**
		 * Makes all library entry paths absolute by appending them to the current resources folder. Entries must have
		 * previously been made relative by calling makeEntriesRelative().
		 */
		void makeEntriesAbsolute();

		/** Deletes all library entries. */
		void clearEntries();

		/** 
		 * Finalizes a queued import operation if the import task has finished (or immediately if no task is present). 
		 *
		 * @param[in]	fileEntry		File entry for which the import is running.
		 * @param[in]	import			Structure containing information about the import.
		 * @param[in]	wait			If true waits until the asynchronous import task finishes before returning. Not
		 *								relevant if the import task is not present (synchronous import).
		 * @return						True if the import was finalized. Will be false if the async import task has not
		 *								yet finished and @p wait is false.
		 */
		bool finishQueuedImport(FileEntry* fileEntry, const QueuedImport& import, bool wait);

		/** 
		 * Checks if there are any queued imports queued for the provided file entry, and if there are waits until they
		 * finish before returning.s
		 */
		void waitForQueuedImport(FileEntry* fileEntry);

		static const char* LIBRARY_ENTRIES_FILENAME;
		static const char* RESOURCE_MANIFEST_FILENAME;

		SPtr<ResourceManifest> mResourceManifest;
		USPtr<DirectoryEntry> mRootEntry;
		Path mProjectFolder;
		Path mResourcesFolder;
		bool mIsLoaded;

		Mutex mQueuedImportMutex;
		UnorderedMap<FileEntry*, SPtr<QueuedImport>> mQueuedImports;

		UnorderedMap<Path, Vector<Path>> mDependencies;
		UnorderedMap<UUID, Path> mUUIDToPath;
	};

	/**	Provides easy access to ProjectLibrary. */
	BS_ED_EXPORT ProjectLibrary& gProjectLibrary();

	/** @} */
}
