
from parser import ExtractedSymbols, ExtractedEnum, ExtractedStruct, ExtractedFunction
from typing import Any, Callable, DefaultDict, Literal, NotRequired, Optional, TypedDict
from pathlib import Path
from dataclasses import dataclass

SymbolType = Literal['enum', 'struct', 'function']

class BaseGenerator:
    def __init__(self, extracted_symbols: ExtractedSymbols):
        self.extracted_symbols = extracted_symbols
        self.output_content: dict[str, list[str]] = dict()

    def generate(self) -> None:
        pass

    def has_symbol(self, symbol: str) -> bool:
        return (
            symbol in self.extracted_symbols.enums or 
            symbol in self.extracted_symbols.structs or 
            symbol in self.extracted_symbols.functions
        )

    def get_symbol_type(self, symbol: str) -> SymbolType:
        if symbol in self.extracted_symbols.enums:
            return 'enum'
        elif symbol in self.extracted_symbols.structs:
            return 'struct'
        elif symbol in self.extracted_symbols.functions:
            return 'function'
        raise ValueError(f'Unknown symbol: {symbol}')

    def _write(self, file_name: str, content: str) -> None:
        if file_name not in self.output_content:
            self.output_content[file_name] = []
        self.output_content[file_name].append(content)

    def write_outputs(self, output_dir: Path) -> None:
        for file_name, content in self.output_content.items():
            (output_dir / file_name).parent.mkdir(parents=True, exist_ok=True)
            with open(output_dir / file_name, 'w') as f:
                f.write("\n".join(content))

    def get_outputs(self) -> dict[str, str]:
        return {file_name: "\n".join(content) for file_name, content in self.output_content.items()}


