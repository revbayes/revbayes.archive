//
//  ImageLoader.swift
//  macgui
//
//  Created by Svetlana Krasikova on 2/8/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class ImageLoader {
    
    private var imageFiles = [ImageFile]()
    private var folder: String
    
    init(folder: String){
        self.folder = folder
        let resourcePath = NSURL(string: Bundle.main.resourcePath!)?.appendingPathComponent(folder)
        let resourcesContent = try! FileManager().contentsOfDirectory(at: resourcePath!, includingPropertiesForKeys: nil, options: FileManager.DirectoryEnumerationOptions.skipsHiddenFiles)
        for url in resourcesContent {
            let imageFile = ImageFile(url: url)
            imageFiles.append(imageFile)
        }
    }
    
    func getImagesCount () -> Int {
        return imageFiles.count
    }
    
    func getImageFileForPathIndex(indexPath: IndexPath) -> ImageFile{
        let imageIndexInImageFiles = indexPath.item
        let imageFile = imageFiles[imageIndexInImageFiles]
        return imageFile
    }
    
}
