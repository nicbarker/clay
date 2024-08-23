const fs = require('fs');
const path = require('path');

let files = ['../clay.h'];

let templates = ['./'];
function readCTemplatesRecursive(directory) {
    fs.readdirSync(directory).forEach(template => {
        const absolute = path.join(directory, template);
        if (fs.statSync(absolute).isDirectory()) return readCTemplatesRecursive(absolute);
        else if (template.endsWith('template.c')) {
            return templates.push(absolute);
        }
    });
}

readCTemplatesRecursive(__dirname);

for (const file of files) {
    const contents = fs.readFileSync(file, 'utf8');
    const lines = contents.split('\n');
    for (let i = 0; i < lines.length; i++) {
        const line = lines[i];
        if (line.startsWith('// __GENERATED__ template')) {
            const [comment, generated, templateOpen, templateNames, ...args] = line.split(" ");
            let matchingEndingLine = -1;
            for (let j = i + 1; j < lines.length; j++) {
                if (lines[j].startsWith('// __GENERATED__ template')) {
                    matchingEndingLine = j;
                    break;
                }
            }
            if (matchingEndingLine !== -1) {
                i++;
                lines.splice(i, matchingEndingLine - (i));
                lines.splice(i, 0, ['#pragma region generated']);
                i++;
                for (const templateName of templateNames.split(',')) {
                    var matchingTemplate = templates.find(t => t.endsWith(`${templateName}.template.c`));
                    if (matchingTemplate) {
                        let templateContents = fs.readFileSync(matchingTemplate, 'utf8');
                        for (const arg of args) {
                            [argName, argValue] = arg.split('=');
                            templateContents = templateContents.replaceAll(`\$${argName}\$`, argValue);
                        }
                        let remainingTokens = templateContents.split('$');
                        if (remainingTokens.length > 1) {
                            console.log(`Error at ${file}:${i}: Template is missing parameter ${remainingTokens[1]}`)
                            process.exit();
                        } else {
                            templateContents = templateContents.split('\n');
                            lines.splice(i, 0, ...templateContents);
                            i += templateContents.length;
                        }
                    } else {
                        console.log(`Error at ${file}:${i + 1}: no template with name ${templateName}.template.c was found.`);
                        process.exit();
                    }
                }
                lines.splice(i, 0, ['#pragma endregion']);
                i++;
            } else {
                console.log(`Error at ${file}:${i + 1}: template was opened and not closed again.`);
                process.exit();
            }
        }
    }
    fs.writeFileSync(file, lines.join('\n'));
}