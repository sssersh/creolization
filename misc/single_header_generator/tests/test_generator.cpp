
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

#include "generator.h"

using namespace one_header_gen;

class generator_test : public ::testing::Test
{
public:
    void SetUp() override
    {
        fs::remove_all(root_dir);

        fs::create_directories(root_dir / input_dir_name / project_name);

        std::ofstream {root_dir / input_dir_name / project_name / main_file_name};
        std::ofstream {template_out_file_path };

        config = std::make_shared<generator_config_t>(
              root_dir
            , project_name
            , input_dir_name
            , main_file_name
            , output_dir_name
            , template_out_file_path
        );

        generator = std::make_shared<generator_t>(*config);
    }

    void TearDown() override
    {
        fs::remove_all(root_dir);
    }

protected:
    const fs::path root_dir = "test_root_dir";
    const std::string_view project_name = "test_project_name";
    const std::string_view input_dir_name = "test_input_dir_name";
    const std::string_view main_file_name = "test_file.cpp";
    const std::string_view output_dir_name = "test_out_dir_name";
    const fs::path template_out_file_path = "test_root_dir/test_template_file.in";

    std::shared_ptr<generator_config_t> config;
    std::shared_ptr<generator_t> generator;
};

TEST_F(generator_test, prepare_out_dir_and_file)
{
    ASSERT_TRUE(!fs::exists(config->get_output_dir_path()));

    ASSERT_NO_THROW(generator->prepare_out_dir_and_file());

    ASSERT_TRUE(fs::exists(config->get_output_dir_path()));
    ASSERT_TRUE(fs::exists(config->get_out_file_path()));
}