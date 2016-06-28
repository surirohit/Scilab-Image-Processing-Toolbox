function bagList=bagStructToList(bag)
    if isstruct(bag)<> %T  then
            error(msprintf("Error: The input argument is not of type bagOfFeatures."));
        end
        fieldNamesActual=["FilePath";"VocabularySize";"StrongestFeatures";"Upright"];
        fieldNamesInput=fieldnames(bag);
        if fieldNamesActual <> fieldNamesInput then
            error(msprintf("Error: The input argument is not of type bagOfFeatures, Wrong field names"));
        end
        bagList=list();
        bagList(1)='bagOfFeatures';
        bagList(2)=bag.FilePath;
        bagList(3)=bag.VocabularySize;
        bagList(4)=bag.StrongestFeatures;
        bagList(5)=bag.Upright;
endfunction
