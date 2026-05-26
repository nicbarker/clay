import argparse
import logging
import json

from pathlib import Path

from generators.base_generator import BaseGenerator
from generators.odin_generator import OdinGenerator
from parser import parse_headers

logger = logging.getLogger(__name__)

GeneratorMap = dict[str, type[BaseGenerator]]
GENERATORS = {
    'odin': OdinGenerator,
}

def main() -> None:
    arg_parser = argparse.ArgumentParser(description='Generate clay bindings')

    # Directories
    arg_parser.add_argument('input_files', nargs='+', type=str, help='Input header files')
    arg_parser.add_argument('--output-dir', type=str, help='Output directory', required=True)
    arg_parser.add_argument('--tmp-dir', type=str, help='Temporary directory')
    
    # Generators
    arg_parser.add_argument('--generator', type=str, choices=list(GENERATORS.keys()), help='Generators to run', required=True)

    # Logging
    arg_parser.add_argument('--verbose', action='store_true', help='Verbose logging')

    args = arg_parser.parse_args()

    log_formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    log_handler = logging.StreamHandler()
    log_handler.setFormatter(log_formatter)
    if args.verbose:
        logging.basicConfig(level=logging.DEBUG, handlers=[log_handler])
    else:
        logging.basicConfig(level=logging.INFO, handlers=[log_handler])

    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    if args.tmp_dir:
        tmp_dir = Path(args.tmp_dir)
    else:
        tmp_dir = output_dir / 'tmp'
    tmp_dir.mkdir(parents=True, exist_ok=True)

    fake_libc_include_path = Path(__file__).parent / 'fake_libc_include'
    input_files = list(fake_libc_include_path.glob('*.h')) + [Path(f) for f in args.input_files]

    logger.info(f'Input files: {input_files}')
    logger.info(f'Output directory: {output_dir}')
    logger.info(f'Temporary directory: {tmp_dir}')
    logger.info(f'Generator: {args.generator}')

    logger.info('Parsing headers')
    extracted_symbols = parse_headers(input_files, tmp_dir)
    with open(tmp_dir / 'extracted_symbols.json', 'w') as f:
        f.write(json.dumps({
            'structs': extracted_symbols.structs,
            'enums': extracted_symbols.enums,
            'functions': extracted_symbols.functions,
        }, indent=2))

    logger.info('Generating bindings')
    generator = GENERATORS[args.generator](extracted_symbols)
    generator.generate()
    logger.debug(f'Generated bindings:')
    # for file_name, content in generator.get_outputs().items():
    #     logger.debug(f'{file_name}:')
    #     logger.debug(content)
    #     logger.debug('\n')
    
    tmp_outputs_dir = tmp_dir / 'generated'
    tmp_outputs_dir.mkdir(parents=True, exist_ok=True)
    generator.write_outputs(tmp_outputs_dir)


if __name__ == '__main__':
    main()