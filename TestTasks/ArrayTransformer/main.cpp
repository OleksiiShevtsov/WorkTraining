#include "utils/parser.h"
#include "utils/printer.h"
#include "core/transformer_factory.h"
#include "core/types.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
            return 1;
        }

        std::string fileName = argv[1];

        at::FileArrayReader<at::types::Container> reader(fileName);

        const at::types::Container arrays = reader.readArrays();

        auto sortingTransformer = at::TransformerFactory<at::types::Container>::createTransformer(at::TransformType::Sorting);
        if (sortingTransformer)
        {
            at::types::Container sortedArrays = sortingTransformer->transform(arrays);
            at::Printer::print(sortedArrays, "sort");
        }

        auto intersectionTransformer = at::TransformerFactory<at::types::Container>::createTransformer(at::TransformType::Intersection);
        if (intersectionTransformer)
        {
            at::types::Container intersectionArrays = intersectionTransformer->transform(arrays);
            at::Printer::print(intersectionArrays, "intersection");
        }

        auto uniqueReverseTransformer = at::TransformerFactory<at::types::Container>::createTransformer(at::TransformType::UniqueReverse);
        if (uniqueReverseTransformer)
        {
            at::types::Container uniqueReverseArrays = uniqueReverseTransformer->transform(arrays);
            at::Printer::print(uniqueReverseArrays, "unique_reverse");
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
