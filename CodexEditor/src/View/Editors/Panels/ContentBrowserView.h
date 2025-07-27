#pragma once

#include <Codex.h>

#include "EditorPanel.h"

namespace codex::editor {
    class FileSystem
    {
    public:
        class Directory;
        class File;

    private:
        std::filesystem::path  m_Root;
        std::vector<Directory> m_Directories;

    public:
        FileSystem(std::filesystem::path root);
    };

    class ContentBrowserView : public EditorPanel
    {
    public:
        using EditorPanel::EditorPanel;

    private:
        std::filesystem::path m_ProjectPath;

    protected:
        void OnInit() override;
        void OnImGuiRender() override;
    };

    class FileSystem::Directory
    {
    public:
        // Not sure if i will need this.
        enum class Type
        {
            User,
            Internal
        };

    private:
        std::filesystem::path                           m_Path;
        std::vector<mem::Shared<FileSystem::File>>      m_Files;
        std::vector<mem::Shared<FileSystem::Directory>> m_SubDirectories;

    public:
        Directory(std::filesystem::path path);
    };

    class FileSystem::File
    {
    public:
        enum class Type
        {
            // TODO: Expand AudioFile and ImageFile e.g., MP3File, WAVFile, PNGFile, JPEGFile...
            Auto, // FileSystem::File will detect the file type, for now, it will detect based off of extensions (naive
                  // approach).
            GenericFile,
            TextFile,
            CxxFile,
            CSFile,
            JsonFile,
            CXProjFile,
            ReflectFile,
            AudioFile,
            ImageFile,
            PrefabFile
        };

    private:
        std::filesystem::path m_Path;
        Type                  m_Type;

    public:
        File(std::filesystem::path path, const Type type = Type::Auto);

    public:
        [[nodiscard]] std::string GetName() const noexcept { return m_Path.filename().string(); }
        [[nodiscard]] std::string GetExtension() const noexcept { return m_Path.extension().string(); }
    };
} // namespace codex::editor
