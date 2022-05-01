module.exports = async ({github, releaseId, fullpathToArtefact}) => {
    try {
        console.log(`Release ID: ${releaseId}`);
        const filename = fullpathToArtefact.split(/[\\\/]/).pop();
        console.log(`Filename: ${filename}`);
        const fs = require('fs').promises;
        const response = await github.rest.repos.uploadReleaseAsset({
            owner: 'Sylvie',
            repo: 'Sambada',
            release_id: releaseId,
            name: filename,
            data: await fs.readFile(fullpathToArtefact)
        });
        console.log(`Response: ${JSON.stringify(response, null, "    ")}`);
        return response;
    } catch (error) {
        console.log(error.message)
    }
}