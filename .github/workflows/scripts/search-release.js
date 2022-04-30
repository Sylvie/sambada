module.exports = async ({github, context, core, tagName}) => {
    const query = `query($owner:String!, $name:String!, $tagName:String!) {
                repository(owner: $owner, name: $name) {
                release(tagName: $tagName) {
                                          id
                                          databaseId
                                          createdAt
                                          isDraft
                                          isPrerelease
                                          publishedAt
                                          releaseAssets(last:20) {
                                            edges {
                                              node {
                                                name
                                                }
                                               }
                                                }
                                    }
                                  }
                                }`;
    const variables = {
        owner: context.repo.owner,
        name: context.repo.repo,
        tagName: tagName
    };
    try {
        const result = await github.graphql(query, variables);
        console.log(result);
        if (result["repository"]["release"] != null) {
            console.log("Found a release");
            core.setOutput("status", "found")
            core.setOutput("result", toJSON(result))
        }
        else
        {
            console.log("No release found");
            core.setOutput("status", "not-found")
        }
        return result
    }
    catch (error)
    {
        console.log(error.message)
        core.setOutput("status", "not-found")
    }
}